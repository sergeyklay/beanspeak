#ifndef PHP_BEANSPEAK_PROPERTIES_H
#define PHP_BEANSPEAK_PROPERTIES_H 1

/* Beanspeak\Client properties */
int beanspeak_client_socket_read(beanspeak_object_t *obj, zval *retval);
int beanspeak_client_host_read(beanspeak_object_t *obj, zval *retval);
int beanspeak_client_port_read(beanspeak_object_t *obj, zval *retval);
int beanspeak_client_timeout_read(beanspeak_object_t *obj, zval *retval);
int beanspeak_client_persistent_read(beanspeak_object_t *obj, zval *retval);
int beanspeak_client_usedTube_read(beanspeak_object_t *obj, zval *retval);
int beanspeak_client_watchedTubes_read(beanspeak_object_t *obj, zval *retval);

#endif // PHP_BEANSPEAK_PROPERTIES_H
