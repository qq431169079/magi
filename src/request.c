#include "request.h"

#include <stdlib.h>


void magi_request_init(magi_request *request)
{
    if (request) {
        request->callback.act       = 0;
        request->callback.userdata  = 0;
        request->callback.addon_max = 1024;
        request->limits.cookies     = 1024 * 16;
        request->limits.params_meta = 1024 * 16;
        request->limits.params_head = 1024 * 16;
        request->limits.params_body = 1024 * 16;
    }
}


static void request_free(magi_request *request)
{
    free(request->cookies);
    free(request->meta);
    free(request->head);
    free(request->body);
    free(request->files);
    free(request->method);
    free(request->address);
    free(request->script);
    free(request->path);
}

static void request_annul(magi_request *request)
{
    request->cookies = 0;
    request->meta    = 0;
    request->head    = 0;
    request->body    = 0;
    request->files   = 0;
    request->method  = 0;
    request->address = 0;
    request->script  = 0;
    request->path    = 0;
}

void magi_request_free(magi_request *request)
{
    if (request) {
        magi_cookies_free(request->cookies);
        magi_params_free(request->meta);
        magi_params_free(request->head);
        magi_params_free(request->body);
        magi_files_free(request->files);
        request_free(request);
        request_annul(request);
    }
}


char *magi_request_meta(magi_request *r, const char *name)
{
    return magi_params_get(r->meta, name);
}

char *magi_request_param(magi_request *r, const char *name)
{
    char *res = magi_params_get(r->body, name);
    if (!res) {
        return magi_params_get(r->head, name);
    }
    return res;
}

char *magi_request_urlparam(magi_request *r, const char *name)
{
    return magi_params_get(r->head, name);
}

magi_file *magi_request_file(magi_request *r, const char *name)
{
    return magi_files_get(r->files, name);
}

char *magi_request_cookie(magi_request *r, const char *name)
{
    magi_cookie *res = magi_cookies_get(r->cookies, name);
    if (!res) {
        return 0;
    }
    return res->data;
}

magi_cookie *magi_request_cookie_complex(magi_request *r, const char *name)
{
    return magi_cookies_get(r->cookies, name);
}
