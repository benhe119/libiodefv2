#include "config.h"

#include <stdlib.h>
#include <assert.h>
#include "iodefv2.h"

#define TEST_STR "abcdefghijklmnopqrstuvwxyz"
#define MAX_LAG_SEC 3


int main(void)
{
        time_t now;
        iodefv2_time_t *ctime;
        iodefv2_incident_t *incident;
        iodefv2_document_t *iodefv2;

        assert(iodefv2_document_new(&iodefv2) == 0);
        assert(iodefv2_document_new_incident(iodefv2, &incident, 1) == 0);

        ctime = iodefv2_incident_get_report_time(incident);
        assert(ctime != NULL);

        now = time(NULL);
        assert(now - iodefv2_time_get_sec(ctime) < MAX_LAG_SEC);
        printf("it's ok \n");
        exit(0);
}
