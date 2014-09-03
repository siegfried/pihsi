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
    VALUE result = rb_hash_new();

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
        rb_hash_aset(result, rb_str_new2("score"), INT2NUM(hyp));
        rb_hash_aset(result, rb_str_new2("hypothesis"), rb_str_new2(hyp));
        return result;
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

VALUE initialize(VALUE self, VALUE options) {
    int i;
    PocketSphinx *ps;
    VALUE option, key, value, klass;
    char *c_key;
    cmd_ln_t *config = cmd_ln_init(NULL, ps_args(), TRUE, "-logfn", "/dev/null", NULL);

    for (i = 0; i < RARRAY_LEN(options); i++) {
        option = rb_ary_entry(options, i);
        key = rb_ary_entry(option, 0);
        c_key = RSTRING_PTR(key);
        value = rb_ary_entry(option, 1);
        klass = rb_funcall(value, rb_intern("class"), 0);

        if (klass == rb_cString) {
            cmd_ln_set_str_r(config, c_key, RSTRING_PTR(value));
        } else if (klass == rb_cFixnum) {
            cmd_ln_set_int_r(config, c_key, NUM2LONG(value));
        } else if (klass == rb_cFloat) {
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
    VALUE rb_mPocketSphinx = rb_define_module_under(rb_mPihsi, "PocketSphinx");
    VALUE rb_cDecoder = rb_define_class_under(rb_mPocketSphinx, "Decoder", rb_cObject);
    rb_define_alloc_func(rb_cDecoder, allocate);
    rb_define_method(rb_cDecoder, "initialize", initialize, 1);
    rb_define_method(rb_cDecoder, "decode", decode, 1);
}
