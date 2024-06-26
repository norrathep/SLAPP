#include <math.h>
#include <stdio.h>
#include <float.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "textgen.h"

extern int heapSize;

#define VERSION "r490"
#define kv_roundup32(x) (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))

tg_data_t *tg_init_with_data(uint8_t *input, int len)
{
	int i, j, st, k;
	tg_data_t *tg;
	tg = (tg_data_t*)calloc(1, sizeof(tg_data_t));
	heapSize += 1* sizeof(tg_data_t);

	tg->len = len;
	tg->data = calloc(tg->len, sizeof(uint8_t));
	heapSize += tg->len * sizeof(uint8_t);
	memcpy(tg->data, input, len);

	for (i = 0; i < tg->len; ++i)
		tg->c2i[tg->data[i]] = 1;
	for (i = j = 0; i < 256; ++i)
		if (tg->c2i[i] == 0) tg->c2i[i] = -1;
		else tg->c2i[i] = j++;
	tg->n_char = j;
	for (i = 0; i < tg->len; ++i)
		tg->data[i] = tg->c2i[tg->data[i]];
	return tg;
}

tg_data_t *tg_init()
{
	int i, j, st, k;
	tg_data_t *tg;
	tg = (tg_data_t*)calloc(1, sizeof(tg_data_t));
	heapSize += 1*sizeof(tg_data_t);

	// OAK: my hack
	tg->len = 32*4;
	char current_char = 'a';
    // Fill the array with characters 'a' to 'z' in a circular manner
	tg->data = calloc(tg->len, sizeof(uint8_t));
	heapSize += tg->len*sizeof(uint8_t);
    for (i = 0; i < tg->len; i++) {
        tg->data[i] = current_char;
        current_char++;
        if (current_char > 'z') {
            current_char = 'a';
        }
    }
	// end of hack


	for (i = 0; i < tg->len; ++i)
		tg->c2i[tg->data[i]] = 1;
	for (i = j = 0; i < 256; ++i)
		if (tg->c2i[i] == 0) tg->c2i[i] = -1;
		else tg->c2i[i] = j++;
	tg->n_char = j;
	for (i = 0; i < tg->len; ++i)
		tg->data[i] = tg->c2i[tg->data[i]];
	return tg;
}

void tg_gen(kann_t *ann, float temp, int len, const int c2i[256])
{
	int i, c, n_char, i2c[256], i_temp;
	float x[256];
	memset(i2c, 0, 256 * sizeof(int));
	for (i = 0; i < 256; ++i)
		if (c2i[i] >= 0) i2c[c2i[i]] = i;
	n_char = kann_dim_in(ann);
	i_temp = kann_find(ann, 0, -1);
	if (i_temp >= 0) ann->v[i_temp]->x[0] = 1.0f / temp;
	kann_rnn_start(ann);
	for (c = 0; c < ann->n; ++c) {
		kad_node_t *p = ann->v[c];
		if (p->pre) {
			int l = kad_len(p);
			for (i = 0; i < l; ++i)
				p->x[i] = 2.0 * kann_drand() - 1.0;
		}
	}
	c= c2i[(int)' '];

	char buffer[20] = {0};

	for (i = 0; i < len; ++i) {
		float s, r;
		const float *y;
		memset(x, 0, n_char * sizeof(float));
		x[c] = 1.0f;
		y = kann_apply1(ann, x);
		r = kann_drand();
		for (c = 0, s = 0.0f; c < n_char; ++c)
			if (s + y[c] >= r) break;
			else s += y[c];

		buffer[i] = i2c[c];
	}

	buffer[len] = '\n';
	sendline(buffer);

	kann_rnn_end(ann);
	if (i_temp >= 0) ann->v[i_temp]->x[0] = 1.0f;
}

int tg_urnn_start(kann_t *ann, int batch_size)
{
	int i, j, n, cnt = 0;
	for (i = 0; i < ann->n; ++i) {
		kad_node_t *p = ann->v[i];
		if (p->pre && p->n_d >= 2 && p->pre->n_d == p->n_d && p->pre->n_child == 0 && kad_len(p)/p->d[0] == kad_len(p->pre)/p->pre->d[0])
			p->pre->flag = 0;
	}
	kann_set_batch_size(ann, batch_size);
	for (i = 0; i < ann->n; ++i) {
		kad_node_t *p = ann->v[i];
		if (p->pre && p->n_d >= 2 && p->pre->n_d == p->n_d && p->pre->n_child == 0 && kad_len(p) == kad_len(p->pre)) {
			kad_node_t *q = p->pre;
			n = kad_len(p) / p->d[0];
			memset(p->x, 0, p->d[0] * n * sizeof(float));
			if (q->x)
				for (j = 0; j < p->d[0]; ++j)
					memcpy(&p->x[j * n], q->x, n * sizeof(float));
			q->x = p->x;
			++cnt;
		}
	}
	return cnt;
}

extern void sendline(char* s);

void tg_train(kann_t *ann, const tg_data_t *tg, float lr, int ulen, int vlen, int cs, int mbs, int max_epoch, float grad_clip, int batch_len, int n_threads)
{
	int i, epoch, u, n_var, n_char;
	float **x, **y, *r;
	const uint8_t **p;
	kann_t *ua;

	sendline("TG_TRAIN 1\n");

	batch_len = batch_len < tg->len? batch_len : tg->len;


	n_char = kann_dim_in(ann);
	x = (float**)calloc(ulen, sizeof(float*));
	heapSize += ulen*sizeof(float*);
	y = (float**)calloc(ulen, sizeof(float*));
	heapSize += ulen*sizeof(float*);

	for (u = 0; u < ulen; ++u) {
		x[u] = (float*)calloc(n_char * mbs, sizeof(float));
		heapSize += n_char * mbs * sizeof(float);
		y[u] = (float*)calloc(n_char * mbs, sizeof(float));
		heapSize += n_char * mbs * sizeof(float);
	}
	n_var = kann_size_var(ann);
	r = (float*)calloc(n_var, sizeof(float));
	heapSize += n_var* sizeof(float);
	p = (const uint8_t**)calloc(mbs, sizeof(const uint8_t*));
	heapSize += mbs* sizeof(const uint8_t*);


	sendline("TG_TRAIN 2\n");

	ua = kann_unroll(ann, ulen);

	sendline("TG_TRAIN 3\n");
	tg_urnn_start(ua, mbs);
	//kann_mt(ua, n_threads, mbs);
	//kann_switch(ua, 1);
	kann_feed_bind(ua, KANN_F_IN,  100, x);
	kann_feed_bind(ua, KANN_F_TRUTH, 0, y);

	sendline("TG_TRAIN 4\n");

	for (epoch = 0; epoch < max_epoch; ++epoch) {
		double cost = 0.0;
		int c, j, b, tot = 0, ctot = 0, n_cerr = 0;

		//sendline("TG_TRAIN 5\n");
		for (i = 0; i < batch_len; i += mbs * cs * ulen) {

			//sendline("TG_TRAIN 6\n");
	
			for (b = 0; b < mbs; ++b)
				p[b] = tg->data + (int)((tg->len - ulen * cs - 1) * kad_drand(0)) + 1;
			for (j = 0; j < ua->n; ++j) // reset initial hidden values to zero
				if (ua->v[j]->pre)
					memset(ua->v[j]->x, 0, kad_len(ua->v[j]) * sizeof(float));
			for (c = 0; c < cs; ++c) {
				int ce_len = c? ulen : ulen - vlen;
				for (u = 0; u < ulen; ++u) {
					memset(x[u], 0, mbs * n_char * sizeof(float));
					memset(y[u], 0, mbs * n_char * sizeof(float));
				}
				//sendline("TG_TRAIN 7\n");
				for (b = 0; b < mbs; ++b) {
					for (u = 0; u < ulen; ++u) {
						x[u][b * n_char + p[b][u-1]] = 1.0f;
						if (c || u >= vlen)
							y[u][b * n_char + p[b][u]] = 1.0f;
					}
					p[b] += ulen;
				}
				//sendline("TG_TRAIN 81\n");
				cost += kann_cost(ua, 0, 1) * ulen * mbs;
				/*sendline("TG_TRAIN 81\n");
				n_cerr += kann_class_error(ua, &b);
				sendline("TG_TRAIN 81\n");
				tot += ce_len * mbs, ctot += b;
				sendline("TG_TRAIN 81\n");
				if (grad_clip > 0.0f) kann_grad_clip(grad_clip, n_var, ua->g);
				sendline("TG_TRAIN 81\n");*/
				kann_RMSprop(n_var, lr, 0, 0.9f, ua->g, ua->x, r);
				//sendline("TG_TRAIN 8\n");
			}
		}

		/*char buffer[50] = {0};
		sprintf(buffer, "epoch: %d; error %d : error %d\n", epoch+1, (int) (100.0 * n_cerr / ctot));
		sendline(buffer);*/


		//fprintf(stderr, "epoch: %d; running cost: %g (class error: %.2f%%)\n", epoch+1, cost / tot, 100.0 * n_cerr / ctot);
		tg_gen(ann, 0.4f, 20, tg->c2i);
		//if (fn) tg_save(fn, ann, tg->c2i);
	}
	kann_delete_unrolled(ua);

	sendline("DONE!\n");

	for (u = 0; u < ulen; ++u) {
		free(x[u]); free(y[u]);
	}

	free(r); free(y); free(x); free(p);
}

kann_t *model_gen(int model, int n_char, int n_h_layers, int n_h_neurons, float h_dropout, int use_norm)
{
	int i, flag = use_norm? KANN_RNN_NORM : 0;
	kad_node_t *t, *t1;
	t = kann_layer_input(n_char), t->ext_label = 100;
	for (i = 0; i < n_h_layers; ++i) {
		//if (model == 0) t = kann_layer_rnn(t, n_h_neurons, flag);
		//else if (model == 1) t = kann_layer_lstm(t, n_h_neurons, flag);
		//else if (model == 2) t = kann_layer_gru(t, n_h_neurons, flag);
		t = kann_layer_lstm(t, n_h_neurons, flag);
		//t = kann_layer_dropout(t, h_dropout);
	}
	t = kann_layer_dense(t, n_char);
	t1 = kann_new_scalar(KAD_CONST, 1.0f), t1->ext_label = -1; // -1 is for backward compatibility
	t = kad_mul(t, t1); // t1 is the inverse of temperature
	t = kad_softmax(t), t->ext_flag |= KANN_F_OUT;
	t1 = kad_feed(2, 1, n_char), t1->ext_flag |= KANN_F_TRUTH;
	t = kad_ce_multi(t, t1), t->ext_flag |= KANN_F_COST;
	return kann_new(t, 0);
}

/*int main() {

		sendline("HELLO\n");

		int c, seed = 11, ulen = 70, vlen = 10, n_h_layers = 1, n_h_neurons = 16, model = 2, max_epoch = 50, mbs = 32;
		int len_gen = 1000, use_norm = 1, batch_len = 1000, n_threads = 1, cs = 100;
		float h_dropout = 0.0f, temp = 0.5f, lr = 0.01f, grad_clip = 0.0f;
		tg_data_t *tg;
		kann_t *ann = 0;
		tg = tg_init();
		sendline("INIT DONE\n");

		ann = model_gen(model, tg->n_char, n_h_layers, n_h_neurons, h_dropout, use_norm);
		sendline("MODEL_GEN DONE\n");

		tg_train(ann, tg, lr, ulen, vlen, cs, mbs, max_epoch, grad_clip, batch_len, n_threads);
		sendline("TG_TRAIN DONE\n");

		free(tg->data); free(tg);
		//tg_gen(ann, temp, len_gen, c2i, prefix);
}*/

/*int main(int argc, char *argv[])
{
	int c, seed = 0, ulen = 1, vlen = 1, n_h_layers = 1, n_h_neurons = 8, model = 0, max_epoch = 10, mbs = 4, c2i[256];
	int len_gen = 1000, use_norm = 1, batch_len = 1000000, n_threads = 1, cal_perp = 0, cs = 100;
	float h_dropout = 0.0f, temp = 0.5f, lr = 0.01f, grad_clip = 0.0f;
	kann_t *ann = 0;
	char *fn_in = 0, *fn_out = 0, *prefix = 0;
	if (vlen >= ulen) vlen = ulen - 1;
	fprintf(stderr, "Version: %s\n", VERSION);
	fprintf(stderr, "Command line:");
	for (c = 0; c < argc; ++c)
		fprintf(stderr, " %s", argv[c]);
	fprintf(stderr, "\n");
	kann_srand(seed);
	kad_trap_fe();
	if (fn_in) ann = tg_load(fn_in, c2i);

	if (argc - optind >= 1) { // train
		tg_data_t *tg;
		tg = tg_init(argv[optind]);
		fprintf(stderr, "Read %d characters; alphabet size %d\n", tg->len, tg->n_char);
		//fprintf(stderr, "Read %d paragraphs and %d characters; alphabet size %d\n", tg->n_para, tg->len, tg->n_char);
		if (!ann) ann = model_gen(model, tg->n_char, n_h_layers, n_h_neurons, h_dropout, use_norm);
		tg_train(ann, tg, lr, ulen, vlen, cs, mbs, max_epoch, grad_clip, fn_out, batch_len, n_threads);
		//if (cal_perp) fprintf(stderr, "Character-level perplexity: %g\n", tg_perplexity(ann, tg));
		free(tg->data); free(tg);
	} else tg_gen(stdout, ann, temp, len_gen, c2i, prefix);

	kann_delete(ann);
	return 0;
}*/
