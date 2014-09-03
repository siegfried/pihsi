#include <ruby.h>
#include <pocketsphinx.h>

typedef struct ps {
	ps_decoder_t *decoder;
} PocketSphinx;

VALUE decode(VALUE self, VALUE data) {
	char const *hyp, *uttid;
	int rv;
	int32 score;
	ps_decoder_t *ps;
	PocketSphinx *pocketSphinx;

	Data_Get_Struct(self, PocketSphinx, pocketSphinx);
	ps = pocketSphinx -> decoder;

	int data_length = RARRAY_LEN(data);
	int16 *c_data = malloc(sizeof(int16) * data_length);

	int i;
	for(i = 0; i < data_length; i++) {
		c_data[i] = NUM2INT(rb_ary_entry(data, i)); // Rubinius does not support NUM2SHORT();
	}

	rv = ps_start_utt(ps, "goforward");

	if (rv < 0) {
		free(c_data);
		rb_raise(rb_eStandardError, "cannot start utterance");
	}

	rv = ps_process_raw(ps, c_data, data_length, FALSE, FALSE);
	rv = ps_end_utt(ps);

	if (rv < 0) {
		free(c_data);
		rb_raise(rb_eStandardError, "cannot end utterance");
	}

	hyp = ps_get_hyp(ps, &score, &uttid);

	free(c_data);

	if (hyp == NULL) {
		return Qnil;
	} else {
		return rb_str_new2(hyp);
	}
}

static void deallocate(void *ps) {
	PocketSphinx *pocketSphinx = ps;
	ps_free(pocketSphinx -> decoder);
	free(pocketSphinx);
}

static VALUE allocate(VALUE self) {
	PocketSphinx *ps;
	return Data_Make_Struct(self, PocketSphinx, 0, deallocate, ps);
}

VALUE initialize(VALUE self, VALUE hmm, VALUE lm, VALUE dict) {
	cmd_ln_t *config = cmd_ln_init(NULL, ps_args(), TRUE, "-hmm", RSTRING_PTR(hmm), "-lm", RSTRING_PTR(lm), "-dict", RSTRING_PTR(dict), "-logfn", "/dev/null", NULL);
	PocketSphinx *ps;
	if (config == NULL)
		rb_raise(rb_eStandardError, "bad configuration");
	Data_Get_Struct(self, PocketSphinx, ps);
	ps -> decoder = ps_init(config);
	return self;
}

void Init_pocket_sphinx() {
	VALUE rb_mPihsi = rb_define_module("Pihsi");
	VALUE rb_mPocketSphinx = rb_define_module_under(rb_mPihsi, "PocketSphinx");
	VALUE rb_cDecoder = rb_define_class_under(rb_mPocketSphinx, "Decoder", rb_cObject);
	rb_define_alloc_func(rb_cDecoder, allocate);
	rb_define_method(rb_cDecoder, "initialize", initialize, 3);
	rb_define_method(rb_cDecoder, "decode", decode, 1);
}
