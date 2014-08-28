#include <ruby.h>
#include <pocketsphinx.h>

VALUE recognize(VALUE self, VALUE data) {
	char const *hyp, *uttid;
	int rv;
	int32 score;
	ps_decoder_t *ps;
	cmd_ln_t *config;

	int data_length = RARRAY_LEN(data);
	int16 *c_data = malloc(sizeof(int16) * data_length);

	char *hmm = RSTRING_PTR(rb_funcall(self, rb_intern("hmm"), 0));
	char *lm = RSTRING_PTR(rb_funcall(self, rb_intern("lm"), 0));
	char *dict = RSTRING_PTR(rb_funcall(self, rb_intern("dict"), 0));

	int i;
	for(i = 0; i < data_length; i++) {
		c_data[i] = NUM2SHORT(rb_ary_entry(data, i));
	}

	config = cmd_ln_init(NULL, ps_args(), TRUE, "-hmm", hmm, "-lm", lm, "-dict", dict, "-logfn", "/dev/null", NULL);
	if (config == NULL)
		rb_raise(rb_eStandardError, "configuration might be wrong");

	ps = ps_init(config);
	rv = ps_start_utt(ps, "goforward");
	if (rv < 0)
		rb_raise(rb_eStandardError, "cannot start utterance");

	rv = ps_process_raw(ps, c_data, data_length, FALSE, FALSE);
	rv = ps_end_utt(ps);
	if (rv < 0)
		rb_raise(rb_eStandardError, "cannot end utterance");
	hyp = ps_get_hyp(ps, &score, &uttid);
	if (hyp == NULL)
		rb_raise(rb_eStandardError, "no hypothesis string");

	free(c_data);
	return rb_str_new2(hyp);
}

void Init_pocket_sphinx() {
	VALUE rb_mPihsi = rb_define_module("Pihsi");
	VALUE rb_mPocketSphinx = rb_define_module_under(rb_mPihsi, "PocketSphinx");
	rb_define_method(rb_mPocketSphinx, "recognize", recognize, 1);
}
