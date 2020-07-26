#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>
#include<curl/curl.h>

#define SECS_PER_YEAR 31536000

int main(int argc, char **argv) {
	CURL *curl;
	FILE *fp;
	time_t cur_time = time(NULL);
	const char *strlwr(const char *str);
	int result;
	const char *filename = "../stock_files/%s.csv";
	char formated_filename[10];
	char url[] = "https://query1.finance.yahoo.com/v7/finance/download/%s?period1=%ld&period2=%ld&interval=1d&events=history";
	char formated_url[140];

	if(argc < 2) {
		puts("Missing argument please enter the ticker of the \n");
		exit(-1);
	}
	if(0 == strcmp(argv[1], "-h") || 0 == strcmp(argv[1], "--help")){
		puts("Some help message");
		exit(0);
	}

	curl = curl_easy_init();
	sprintf(formated_url, url, argv[1], cur_time - SECS_PER_YEAR, cur_time);
	sprintf(formated_filename, filename, strlwr(argv[1]));

	fp = fopen(formated_filename, "wb");
	curl_easy_setopt(curl, CURLOPT_URL, formated_url);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

	result = curl_easy_perform(curl);

	if(result == CURLE_OK){
		printf("Download successful!\n");
	} else {
		printf("ERROR: %s\n", curl_easy_strerror(result));
	}
	fclose(fp);
	curl_easy_cleanup(curl);
	return 0;
}


const char *strlwr(const char *str) {
	unsigned char *p = (unsigned char *)str;

	while(*p){
	 *p = tolower((unsigned char)*p);
	 p++;
	}
	return str;
}

