#include <libiodefv2/iodefv2.h>
#include <string.h>


int main(){
    FILE *file;

    file = fopen("/tmp/example.iodefv2", "a");
    libiodefv2_io_t *pio;
    int ret;

    libiodefv2_io_new(&pio);
    libiodefv2_io_set_file_io(pio, file);

    iodefv2_document_t *iodefv2;
    iodefv2_incident_t *incident;
    iodefv2_incident_id_t *incident_id;
    libiodefv2_string_t *str;

    ret = iodefv2_document_new(&iodefv2);
    if ( ret < 0 ) {
            libiodefv2_perror(ret, "unable to create IODEFV2 message");
            return -1;
    }

    ret = iodefv2_document_new_incident(iodefv2, &incident, 0);
    if ( ret < 0 ) {
            libiodefv2_perror(ret, "unable to create IODEFV2 alert");
            iodefv2_document_destroy(iodefv2);
            return -1;
    }

    ret = iodefv2_incident_new_incident_id(incident, &incident_id);
    if ( ret < 0 ) {
            libiodefv2_perror(ret, "unable to create IODEFV2 alert");
            iodefv2_document_destroy(iodefv2);
            return -1;
    }

    ret = iodefv2_incident_id_new_name(incident_id, &str);
    if ( ret < 0 ) {
            libiodefv2_perror(ret, "unable to create IODEFV2 alert");
            iodefv2_document_destroy(iodefv2);
            return -1;
    }
    libiodefv2_string_set_constant(str, "Incident ID");

    iodefv2_document_set_string(iodefv2, "incident(0).incident_id.name", "Incident ID");
    iodefv2_document_set_string(iodefv2, "incident(1).incident_id.name", "Another Incident ID");

    iodefv2_time_t *time;
    iodefv2_time_new_from_gettimeofday(&time);

    iodefv2_incident_set_report_time(incident, time);

    iodefv2_document_print(iodefv2, pio);
    iodefv2_document_write(iodefv2, pio);
}
