#include <ruby.h>
#include <pocketsphinx.h>

typedef struct ps {
    ps_decoder_t *decoder;
} PocketSphinx;

static VALUE rb_eUtteranceError;

static VALUE decode(ps_decoder_t *ps, VALUE data) {
    char const *hyp, *uttid;
    int rv;
    int32 score;

    rv = ps_start_utt(ps, "goforward");

    if (rv < 0)
        rb_raise(rb_eUtteranceError, "cannot start utterance");

    rv = ps_process_raw(ps, (int16 *) RSTRING_PTR(data), RSTRING_LEN(data) / 2, FALSE, FALSE);
    rv = ps_end_utt(ps);

    if (rv < 0)
        rb_raise(rb_eUtteranceError, "cannot end utterance");

    hyp = ps_get_hyp(ps, &score, &uttid);

    if (hyp == NULL) {
        return Qnil;
    } else {
        return rb_str_new2(hyp);
    }
}

/* Converts raw audio data into text.
 *
 * @param data [String, #read] the raw audio data or its io object
 * @return [String, nil] the transcribed text or nil
 */
VALUE recognize(VALUE self, VALUE data) {
    VALUE string;
    PocketSphinx *pocketSphinx;

    Data_Get_Struct(self, PocketSphinx, pocketSphinx);
    if (rb_funcall(data, rb_intern("respond_to?"), 1, rb_str_new2("read")) == Qtrue) {
        string = rb_funcall(data, rb_intern("read"), 0);
    } else if (rb_obj_is_kind_of(data, rb_cString)) {
        string = data;
    } else {
        rb_raise(rb_eArgError, "data can only be a string or an IO object");
    }
    return decode(pocketSphinx -> decoder, string);
}

static void deallocate(PocketSphinx *pocketSphinx) {
    ps_free(pocketSphinx -> decoder);
    free(pocketSphinx);
}

static VALUE allocate(VALUE self) {
    PocketSphinx *ps;
    return Data_Make_Struct(self, PocketSphinx, 0, deallocate, ps);
}

/*
 * @param options [Hash] PocketSphinx configuration (e.g. hmm)
 */
VALUE initialize(VALUE self, VALUE options_hash) {
    int i;
    PocketSphinx *ps;
    VALUE option, key, value;
    char *c_key;
    cmd_ln_t *config = cmd_ln_init(NULL, ps_args(), TRUE, "-logfn", "/dev/null", NULL);

    VALUE options = rb_funcall(options_hash, rb_intern("to_a"), 0);

    for (i = 0; i < RARRAY_LEN(options); i++) {
        option = rb_ary_entry(options, i);
        key = rb_funcall(rb_ary_entry(option, 0), rb_intern("to_s"), 0);
        key = rb_funcall(key, rb_intern("prepend"), 1, rb_str_new2("-"));
        c_key = RSTRING_PTR(key);
        value = rb_ary_entry(option, 1);

        if (rb_obj_is_kind_of(value, rb_cString)) {
            cmd_ln_set_str_r(config, c_key, RSTRING_PTR(value));
        } else if (rb_obj_is_kind_of(value, rb_cFixnum)) {
            cmd_ln_set_int_r(config, c_key, NUM2LONG(value));
        } else if (rb_obj_is_kind_of(value, rb_cFloat)) {
            cmd_ln_set_float_r(config, c_key, NUM2DBL(value));
        }
    }

    if (config == NULL)
        rb_raise(rb_eStandardError, "bad configuration");

    Data_Get_Struct(self, PocketSphinx, ps);
    ps -> decoder = ps_init(config);
    return self;
}

void Init_pocket_sphinx() {
    VALUE rb_mPihsi = rb_define_module("Pihsi");
    VALUE rb_cSpeechRecognizer = rb_define_class_under(rb_mPihsi, "SpeechRecognizer", rb_cObject);
    rb_define_alloc_func(rb_cSpeechRecognizer, allocate);
    rb_define_method(rb_cSpeechRecognizer, "initialize", initialize, 1);
    rb_define_method(rb_cSpeechRecognizer, "recognize", recognize, 1);
    rb_eUtteranceError = rb_define_class_under(rb_mPihsi, "UtteranceError", rb_eStandardError);
}
