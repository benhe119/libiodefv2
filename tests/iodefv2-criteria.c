#include "config.h"

#include <stdlib.h>
#include <assert.h>
#include "iodefv2.h"

static void test_criteria(iodefv2_document_t *iodefv2, const char *criteria_str, int expect_create, int expect_match)
{
        iodefv2_criteria_t *criteria;

        if ( expect_create < 0 ) {
                assert(iodefv2_criteria_new_from_string(&criteria, criteria_str) < 0);
                return;
        } else
                assert(iodefv2_criteria_new_from_string(&criteria, criteria_str) == 0);
        assert(iodefv2_criteria_match(criteria, iodefv2) == expect_match);
        iodefv2_criteria_destroy(criteria);

}

int main(void)
{
        iodefv2_time_t *ctime;
        iodefv2_incident_t *incident;
        iodefv2_document_t *iodefv2;
        iodefv2_event_data_t *event_data;
        libiodefv2_string_t *str;

        assert(libiodefv2_string_new_ref(&str, "A") == 0);

        assert(iodefv2_document_new(&iodefv2) == 0);
        assert(iodefv2_document_new_incident(iodefv2, &incident, 1) == 0);
        assert(iodefv2_incident_new_event_data(incident, &event_data, 1) == 0);
        iodefv2_event_data_set_description(event_data, str);

        iodefv2_document_set_string(iodefv2, "incident.method(0).description", "A");
        iodefv2_document_set_string(iodefv2, "incident.method(1).description", "B");
        iodefv2_document_set_string(iodefv2, "incident.method(0).reference(0).url", "a0");
        iodefv2_document_set_string(iodefv2, "incident.method(1).reference(1).url", "a2");
        test_criteria(iodefv2, "incident", 0, 1);
        test_criteria(iodefv2, "incident.event_data.txt == A", -1, -1);
        libiodefv2_string_set_ref(str, "My String");
        test_criteria(iodefv2, "incident.event_data.description != 'My String'", 0, 0);
        test_criteria(iodefv2, "incident.event_data.description != 'random'", 0, 1);
        test_criteria(iodefv2, "incident.event_data.description == 'My String'", 0, 1);
        test_criteria(iodefv2, "incident.event_data.description <> 'My'", 0, 1);
        test_criteria(iodefv2, "incident.event_data.description <> 'my'", 0, 0);
        test_criteria(iodefv2, "incident.event_data.description <>* 'my'", 0, 1);
        test_criteria(iodefv2, "incident.event_data.description ~ 'My String'", 0, 1);
        test_criteria(iodefv2, "incident.event_data.description ~ 'My (String|Check)'", 0, 1);
        test_criteria(iodefv2, "incident.event_data.description ~ 'my'", 0, 0);
        test_criteria(iodefv2, "incident.event_data.description ~* 'my'", 0, 1);
        iodefv2_incident_new_report_time(incident, &ctime);
        assert(iodefv2_time_set_from_string(ctime, "2015-05-03 1:59:08") == 0);
        /*
         * Regular time operator check
         */
        test_criteria(iodefv2, "incident.report_time == '2015-05-03 1:59:08'", 0, 1);
        test_criteria(iodefv2, "incident.report_time != '2015-05-03 1:59:08'", 0, 0);
        test_criteria(iodefv2, "incident.report_time < '2015-05-03 1:59:08'", 0, 0);
        test_criteria(iodefv2, "incident.report_time > '2015-05-03 1:59:08'", 0, 0);
        test_criteria(iodefv2, "incident.report_time <= '2015-05-03 1:59:08'", 0, 1);
        test_criteria(iodefv2, "incident.report_time >= '2015-05-03 1:59:08'", 0, 1);

        test_criteria(iodefv2, "incident.report_time == '2015-05-03 1:59:07'", 0, 0);
        test_criteria(iodefv2, "incident.report_time != '2015-05-03 1:59:07'", 0, 1);
        test_criteria(iodefv2, "incident.report_time < '2015-05-03 1:59:07'", 0, 0);
        test_criteria(iodefv2, "incident.report_time > '2015-05-03 1:59:07'", 0, 1);
        test_criteria(iodefv2, "incident.report_time <= '2015-05-03 1:59:07'", 0, 0);
        test_criteria(iodefv2, "incident.report_time >= '2015-05-03 1:59:07'", 0, 1);

        test_criteria(iodefv2, "incident.report_time < '2015-05-03 1:59:09'", 0, 1);
        test_criteria(iodefv2, "incident.report_time > '2015-05-03 1:59:09'", 0, 0);
        test_criteria(iodefv2, "incident.report_time <= '2015-05-03 1:59:09'", 0, 1);
        test_criteria(iodefv2, "incident.report_time >= '2015-05-03 1:59:09'", 0, 0);
        /*
         * Broken down time check
         * 
         */
        assert(iodefv2_time_set_from_string(ctime, "2015-05-04 00:00:00+00:00") == 0);
        test_criteria(iodefv2, "incident.report_time == 'month:may mday:3'", 0, 0);
        test_criteria(iodefv2, "incident.report_time != 'month:may mday:3'", 0, 1);
        test_criteria(iodefv2, "incident.report_time < 'month:may mday:3'", 0, 0);
        test_criteria(iodefv2, "incident.report_time > 'month:may mday:3'", 0, 1);
        test_criteria(iodefv2, "incident.report_time <= 'month:may mday:3'", 0, 0);
        test_criteria(iodefv2, "incident.report_time >= 'month:may mday:3'", 0, 1);

        test_criteria(iodefv2, "incident.report_time == 'month:may mday:4'", 0, 1);
        test_criteria(iodefv2, "incident.report_time != 'month:may mday:4'", 0, 0);
        test_criteria(iodefv2, "incident.report_time < 'month:may mday:4'", 0, 0);
        test_criteria(iodefv2, "incident.report_time > 'month:may mday:4'", 0, 0);
        test_criteria(iodefv2, "incident.report_time <= 'month:may mday:4'", 0, 1);
        test_criteria(iodefv2, "incident.report_time >= 'month:may mday:4'", 0, 1);

        test_criteria(iodefv2, "incident.report_time == 'month:may mday:5'", 0, 0);
        test_criteria(iodefv2, "incident.report_time != 'month:may mday:5'", 0, 1);
        test_criteria(iodefv2, "incident.report_time < 'month:may mday:5'", 0, 1);
        test_criteria(iodefv2, "incident.report_time > 'month:may mday:5'", 0, 0);
        test_criteria(iodefv2, "incident.report_time <= 'month:may mday:5'", 0, 1);
        test_criteria(iodefv2, "incident.report_time >= 'month:may mday:5'", 0, 0);
        /*
         * Broken down time special wday/yday fields
         */
        test_criteria(iodefv2, "incident.report_time == 'wday:monday'", 0, 1);
        test_criteria(iodefv2, "incident.report_time != 'wday:monday'", 0, 0);
        test_criteria(iodefv2, "incident.report_time == 'wday:tuesday'", 0, 0);
        test_criteria(iodefv2, "incident.report_time != 'wday:tuesday'", 0, 1);
        test_criteria(iodefv2, "incident.report_time == 'wday:monday mday:3'", 0, 0);
        test_criteria(iodefv2, "incident.report_time != 'wday:monday mday:3'", 0, 1);
        test_criteria(iodefv2, "incident.report_time < 'wday:monday mday:3'", 0, 0);
        test_criteria(iodefv2, "incident.report_time > 'wday:monday mday:3'", 0, 1);
        test_criteria(iodefv2, "incident.report_time <= 'wday:monday mday:3'", 0, 0);
        test_criteria(iodefv2, "incident.report_time >= 'wday:monday mday:3'", 0, 1);

        test_criteria(iodefv2, "incident.report_time == 'wday:monday mday:4'", 0, 1);
        test_criteria(iodefv2, "incident.report_time != 'wday:monday mday:4'", 0, 0);
        test_criteria(iodefv2, "incident.report_time < 'wday:monday mday:4'", 0, 0);
        test_criteria(iodefv2, "incident.report_time > 'wday:monday mday:4'", 0, 0);
        test_criteria(iodefv2, "incident.report_time <= 'wday:monday mday:4'", 0, 1);
        test_criteria(iodefv2, "incident.report_time >= 'wday:monday mday:4'", 0, 1);

        test_criteria(iodefv2, "incident.report_time == 'wday:monday mday:5'", 0, 0);
        test_criteria(iodefv2, "incident.report_time != 'wday:monday mday:5'", 0, 1);
        test_criteria(iodefv2, "incident.report_time < 'wday:monday mday:5'", 0, 1);
        test_criteria(iodefv2, "incident.report_time > 'wday:monday mday:5'", 0, 0);
        test_criteria(iodefv2, "incident.report_time <= 'wday:monday mday:5'", 0, 1);
        test_criteria(iodefv2, "incident.report_time >= 'wday:monday mday:5'", 0, 0);
        /*
         * Test on listed object without specific index
         */
        test_criteria(iodefv2, "incident.method(*).description == 'A'", 0, 1);
        test_criteria(iodefv2, "incident.method(*).description != 'A'", 0, 0);
        test_criteria(iodefv2, "incident.method(*).description == 'NOT EXIST'", 0, 0);
        test_criteria(iodefv2, "incident.method(*).description", 0, 1);
        test_criteria(iodefv2, "! incident.method(*).description", 0, 0);
        test_criteria(iodefv2, "incident.method(*).description", 0, 1);
        test_criteria(iodefv2, "incident.method", 0, 1);
        test_criteria(iodefv2, "! incident.method", 0, 0);

        test_criteria(iodefv2, "incident.method(*).reference.url", 0, 1);
        test_criteria(iodefv2, "! Incident.method(*).reference.url", 0, 0);
        test_criteria(iodefv2, "incident.method(0).reference(0).url == 'a0'", 0, 1);
        test_criteria(iodefv2, "incident.method(*).reference(*).url != 'a0'", 0, 0);
        test_criteria(iodefv2, "incident.method(*).reference.url == 'NOT EXIST'", 0, 0);
        test_criteria(iodefv2, "incident.method(*).reference.url != 'NOT EXIST'", 0, 1);
        iodefv2_document_destroy(iodefv2);
        printf("Successful test \n");
        exit(0);
}
