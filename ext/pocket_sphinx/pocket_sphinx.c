#include <ruby.h>
#include <pocketsphinx.h>

VALUE recognize(VALUE self, VALUE string_buffer) {
	char const *hyp, *uttid;
	int rv;
	int32 score;
	ps_decoder_t *ps;
	cmd_ln_t *config;

	char *hmm = RSTRING_PTR(rb_funcall(self, rb_intern("hmm"), 0));
	char *lm = RSTRING_PTR(rb_funcall(self, rb_intern("lm"), 0));
	char *dict = RSTRING_PTR(rb_funcall(self, rb_intern("dict"), 0));
	config = cmd_ln_init(NULL, ps_args(), TRUE, "-hmm", hmm, "-lm", lm, "-dict", dict, "-logfn", "/dev/null", NULL);
	if (config == NULL)
		rb_raise(rb_eStandardError, "configuration might be wrong");

	ps = ps_init(config);
	rv = ps_start_utt(ps, "goforward");
	if (rv < 0)
		rb_raise(rb_eStandardError, "cannot start utterance");

	rv = ps_process_raw(ps, (const int16 *) RSTRING_PTR(string_buffer), RSTRING_LEN(string_buffer) / 2, FALSE, FALSE);
	rv = ps_end_utt(ps);
	if (rv < 0)
		rb_raise(rb_eStandardError, "cannot end utterance");
	hyp = ps_get_hyp(ps, &score, &uttid);
	if (hyp == NULL)
		rb_raise(rb_eStandardError, "no hypothesis string");
	return rb_str_new2(hyp);
}

void Init_pocket_sphinx() {
	VALUE rb_cPihsiModule = rb_define_module("Pihsi");
	VALUE rb_cPocketSphinxModule = rb_define_module_under(rb_cPihsiModule, "PocketSphinx");
	rb_define_method(rb_cPocketSphinxModule, "recognize", recognize, 1);
}
