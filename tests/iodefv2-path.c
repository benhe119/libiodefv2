#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "iodefv2.h"

static void set_value_check(iodefv2_document_t *iodefv2, const char *paths,
                            const char *str_value, libiodefv2_bool_t verify_get)
{
        iodefv2_path_t *path;
        iodefv2_value_t *value;
        libiodefv2_string_t *str;
        libiodefv2_string_t *res;

        assert(iodefv2_path_new_fast(&path, paths) == 0);
        assert(libiodefv2_string_new_ref(&str, str_value) == 0);
        assert(iodefv2_value_new_string(&value, str) == 0);

        if ( verify_get )
                assert(iodefv2_path_set(path, iodefv2, value) == 0);
        else
                assert(iodefv2_path_set(path, iodefv2, value) < 0);

        iodefv2_value_destroy(value);

        if ( ! verify_get ) {
                iodefv2_path_destroy(path);
                return;
        }

        assert(iodefv2_path_get(path, iodefv2, &value) > 0);

        assert(libiodefv2_string_new(&res) == 0);
        assert(iodefv2_value_to_string(value, res) >= 0);
        assert(strcmp(str_value, libiodefv2_string_get_string(res)) == 0);
        libiodefv2_string_destroy(res);

        iodefv2_value_destroy(value);
        iodefv2_path_destroy(path);
}

int main(void)
{
        int i, ret;
        iodefv2_value_t *value;
        iodefv2_path_t *path;
        iodefv2_document_t *iodefv2;
        struct {
                const char *path;
                int depth;
                libiodefv2_bool_t has_list;
                libiodefv2_bool_t ambiguous;
                libiodefv2_bool_t successful;
        } plist[] = {
                { "incident(0).event_data(0).description", 3, TRUE, FALSE, TRUE },
                { "incident(0).assessment(0).impact(0).ext_type", 4, TRUE, FALSE, TRUE },
                { "incident(0).event_data(0).method(0).description", 4, TRUE, FALSE, TRUE },
                { "incident(0).method(0).description", 3, TRUE, FALSE, TRUE },
                { "incident(0).invalid.path", 0, FALSE, TRUE, FALSE }
        };

        assert(iodefv2_document_new(&iodefv2) == 0);

        for ( i = 0; i < sizeof(plist) / sizeof(*plist); i++ ) {
                ret = iodefv2_path_new_fast(&path, plist[i].path);
                assert((plist[i].successful == TRUE && ret == 0) || (plist[i].successful == FALSE && ret < 0));
                 
                if ( ret < 0 )
                        continue;
                //printf("name of path is %s\n", iodefv2_path_get_name(path, -1));
                assert(strcmp(plist[i].path, iodefv2_path_get_name(path, -1)) == 0);
                assert(iodefv2_path_get_depth(path) == plist[i].depth);
                assert((iodefv2_path_has_lists(path) > 0 && plist[i].has_list) || (iodefv2_path_has_lists(path) == 0 && ! plist[i].has_list) );
                assert(iodefv2_path_is_ambiguous(path) == plist[i].ambiguous);

                if ( plist[i].ambiguous ) {
                        iodefv2_path_destroy(path);
                        continue;
                }

                /*
                 * Check whether setting NULL value work.
                 */
                ret = iodefv2_path_set(path, iodefv2, NULL);
                assert(ret == 0);
                ret = iodefv2_path_get(path, iodefv2, &value);
                assert(ret == 0); /* empty value */

                iodefv2_path_destroy(path);
        }

        
        set_value_check(iodefv2, "incident(0).assessment(0).impact(0).severity", "high", TRUE);
        set_value_check(iodefv2, "incident(0).assessment(0).impact(0).severity", "Invalid enumeration", FALSE);
        iodefv2_document_destroy(iodefv2);
        printf("successful test \n");
        exit(0);
}
