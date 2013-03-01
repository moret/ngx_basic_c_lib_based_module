#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static u_char directive[] = "my_directive";
static u_char response_body[] = "hello, world";
static u_char content_type[] = "text/html";

static char *setup(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t handler(ngx_http_request_t *r);

static ngx_command_t commands[] = {
    {
        ngx_string(directive),
        NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
        setup,
        0,
        0,
        NULL
    },
    ngx_null_command
};

static ngx_http_module_t context = {
    NULL, NULL,
    NULL, NULL,
    NULL, NULL,
    NULL, NULL
};

ngx_module_t ngx_basic_c_lib_based_module = {
    NGX_MODULE_V1,
    &context,
    commands,
    NGX_HTTP_MODULE,
    NULL, NULL,
    NULL, NULL,
    NULL, NULL,
    NULL, NGX_MODULE_V1_PADDING
};

static char *setup(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = handler;

    return NGX_CONF_OK;
}

static ngx_int_t handler(ngx_http_request_t *r)
{
    ngx_buf_t *b;
    ngx_chain_t out;

    ngx_http_discard_request_body(r);

    r->headers_out.content_type_len = sizeof(content_type) - 1;
    r->headers_out.content_type.len = sizeof(content_type) - 1;
    r->headers_out.content_type.data = content_type;

    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

    out.buf = b;
    out.next = NULL;

    b->pos = response_body;
    b->last = response_body + sizeof(response_body) - 1;
    b->memory = 1;
    b->last_buf = 1;

    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = sizeof(response_body) - 1;
    ngx_http_send_header(r);
    return ngx_http_output_filter(r, &out);
}
