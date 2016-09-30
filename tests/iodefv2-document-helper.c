#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "iodefv2.h"

#define FLOAT_TOLERANCE 0.0001

#ifndef ABS
# define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

#ifndef MAX
# define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif


static double reldif(double a, double b)
{
        double c = ABS(a);
        double d = ABS(b);

        d = MAX(c, d);

        return d == 0.0 ? 0.0 : ABS(a - b) / d;
}


int main(void)
{
        char *res;
        char *resd;
        iodefv2_document_t *iodefv2;

        assert(iodefv2_document_new(&iodefv2) == 0);
        assert(iodefv2_document_set_string(iodefv2, "incident(0).event_data(0).description", "Random value") == 0);
        assert(iodefv2_document_get_string(iodefv2, "incident(0).event_data(0).description", &res) > 0);
        assert(strcmp("Random value", res) == 0);
        free(res);

        assert(iodefv2_document_set_string(iodefv2, "incident(0).assessment(0).impact(0).severity", "high") == 0);
        assert(iodefv2_document_get_string(iodefv2, "incident(0).assessment(0).impact(0).severity", &res) > 0);
        assert(strcmp("high", res) == 0);
        free(res);

        assert(iodefv2_document_set_string(iodefv2, "incident(0).assessment(0).impact(0).severity", "Random value") < 0);
        printf("successful test \n");
        exit(0);
}
