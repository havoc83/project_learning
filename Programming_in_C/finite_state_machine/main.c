#include<stdio.h>
#include<ctype.h>

#define MAX_INPUT 50

typedef enum {start, build_num, number, build_id, identifier, stop} state;

state transition(state curr, char trans);

int main() {
	char transition_char;
	state current_state = start;

	do {
		if (current_state == identifier){
			printf(" - Identifier\n");
			current_state = start;
		} else if (current_state == number) {
			printf(" - Number\n");
			current_state = start;
		}
		scanf("%c", &transition_char);
		if (transition_char != ' ' && transition_char != '.'){
			printf("%c", transition_char);
		}
		current_state = transition(current_state,
					   transition_char);
	} while (current_state != stop);
	return 0;
}

state transition(state curr, char trans){
	if (trans == '.' && (curr == start || curr == number || curr == identifier)){
		return stop;
	}
	if(trans == ' ') {
		if (curr == build_num) {
		 return number;
		} else if (curr == build_id) {
		 return identifier;
		} else {
		 return start;
		}
	} else if (curr == build_num) {
		return build_num;
	} else if (curr == build_id) {
		return build_id;
	} else {
		if(isdigit(trans)){
			return build_num;
		} else if (isalpha(trans)){
			return build_id;
		}
	}

}
