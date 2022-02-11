#include "rpg42.h"

#define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지

void    parse_status(t_status *status, JSON_Object *json);
t_event_day *parse_event(t_event_day *event, JSON_Object *json);
void *read_subject_from_json(char *sub_file, char *sub_name);
void *read_peer_event_from_json(char *sub_file, char *sub_name, int *count);
void *read_peer_role_from_json(char *sub_file, char *sub_name, int *count);
void *read_peer_status_from_json(char *sub_file, char *sub_name, int *count);
void *init_peers();

const char* EXAM_FILE			= "exam.json";
const char* RUSH_FILE			= "rush.json";
const char* PERSONAL_FILE		= "personal.json";
const char* PEER_EVENT_FILE		= "peer_event.json";
const char* PEER_STATUS_FILE	= "peer_status.json";
const char* PEER_ROLE_FILE		= "peer_role.json";

t_user		*init_user(char type) {
	t_user      *user;
	t_status    *status;
	int     error = 0;

	user = (t_user *)malloc(sizeof(t_user));
	status = (t_status *)malloc(sizeof(t_status));
	user->status = status;
	print_header();
	printf("       유저의 이름을 입력해 주세요(최대 20자) : ");
	scanf("%s", user->user_name);
	print_footer();
	user->status->level				= 1;
	user->status->exp				= 0;
	// 능력치 기본 10 + a(전공자)
	if (type == 'a')
	{
		user->type                      = 전공자;
		user->status->intel 	    	= 15;
		user->status->dex	        	= 5;
		user->status->luck		        = 10;
		user->status->mental	        = 10;
		user->status->activ_point       = 10;
		user->status->fame		    	= 0;
		user->status->fighting_point	= 0;
	}
	else if (type == 'b')
	{	
		user->type                      = 비전공자;
		user->status->intel 	    	= 1;
		user->status->dex	        	= 1;
		user->status->luck		        = 10;
		user->status->mental	        = 10;
		user->status->activ_point       = 10;
		user->status->fame		    	= 0;
		user->status->fighting_point	= 0;
	}
	user->sub_list = init_subjet_list();
	return user;
}

t_event_day	**init_event_day() {
	t_event_day **event_day;
	JSON_Value  *rootValue;
	JSON_Object *rootObject;
	JSON_Object *event;
	JSON_Object *status;
	char path[20];
	char num[10];
	char *file;
	
	event_day = (t_event_day **)malloc(sizeof(t_event_day *));
	file = (char *)malloc(sizeof(char) * 20);
	strcat(path, EVENT_DAY_PATH);
	for (int i = 0; i < EVENT_DAY_MAX; i++)
	{
		event_day[i] = (t_event_day *)malloc(sizeof(t_event_day));
		event_day[i]->event_title = (char *)malloc(sizeof(char) * 30);
		event_day[i]->event_content = (char *)malloc(sizeof(char) * 1024);
		sprintf(num, "%d", i + 1);
		strcpy(file, path);
		strcat(file, num);
		strcat(file, ".json");
		rootValue = json_parse_file(file);
		rootObject = json_value_get_object(rootValue);
		event  = json_object_get_object(rootObject, "event");
		status = json_object_get_object(rootObject, "status");
		parse_event(event_day[i], event);
		printf("%s\n", event_day[0]->event_content);
		parse_status(&(event_day[i]->reward_status),status);
		
		json_value_free(rootValue);
	}
	free(file);
	  return event_day;
}

// t_action	*init_action() {
// 	t_action	*action[ACTION_MAX];
// 	char		*filePath;
// 	char		*doc;
 
// 	action = (t_action **)malloc(sizeof(t_action *));
// 	filePath = strcat(JSON_PATH, ACTION_FILE);
// 	doc = readFile(filePath, &size);
// 	if (doc == NULL)
// 		return -1;
// 	JSON json = { 0, };    // JSON 구조체 변수 선언 및 초기화
// 	parseJSON(doc, size, &json);
// 	event_day[0] = json.tokens[0].string;
// 	event_day[1] = json.tokens[1].string;
// 	for (int j = 2; j < 11; j++) {
// 		event_day[2] = atoi(json.tokens[1].string);
// 	}
// 	freeJSON(&json);    // json 안에 할당된 동적 메모리 해제
// 	free(doc);    // 문서 동적 메모리 해제
// 	return event_day;
// }

//t_day **init_scheudule(){
//	t_day 		**work;
//	JSON_Value	*rootValue;
//	JSON_Object	*rootObject;
//	JSON_Array	*array;
//	JSON_Value	*branchVal;
//	JSON_Object	*branchObj;
//	char		path[20];
//	char		num[10];
//	char		*file;
//	int			count;

//	file = (char *)malloc(sizeof(char) * 20);
//	strcpy(file, JSON_PATH);
//	strcat(file, ACTION_FILE);

//	rootValue = json_parse_file(file);
//	rootObject = json_value_get_object(rootValue);
//	array = json_object_get_array(rootObject, "work");
//	count = json_array_get_count(array);
//	work = (t_day **)malloc(sizeof(t_day *) * count);
//	// for (int i = 0; i < count; i++)
//	// {
//	// 	work[i] = (t_day *)malloc(sizeof(t_day));
//	// 	branchVal = json_array_get_value(array, i);
//	// 	branchObj = json_value_get_object(branchVal);
//	// 	work[i]->action->type		= (int)json_object_get_number(branchObj, "type");
//	// 	work[i]->action->distractor = (char *)malloc(sizeof(char) * 1024);
//	// 	strcpy(work[i]->action->distractor (char *)json_object_get_string(branchObj, "distractor"));
//	// }
//	json_value_free(rootValue);
//	free(file);
//	return work;
//}

t_event_day *parse_event(t_event_day *event, JSON_Object *json){
	char    *title;
	char    *content;

	event->day = (int)json_object_get_number(json, "day");
	title = (char *)json_object_get_string(json, "title");
	content= (char *)json_object_get_string(json, "content");
	strcpy(event->event_title, title);
	strcpy(event->event_content, content);
	return event;
}

t_subject_list *init_subjet_list(){
	t_subject_list	*subject;
	char		path[20];
	char		num[10];
	char		*file;
	
	file = (char *)malloc(sizeof(char) * 20);
	subject = (t_subject_list *)calloc(1, sizeof(t_subject_list));
	//pseronal
	strcpy(file, JSON_PATH);
	strcat(file, PERSONAL_FILE);
	subject->personal = read_subject_from_json(file, "personal");
	//exam
	strcpy(file, JSON_PATH);
	strcat(file, EXAM_FILE);
	subject->exam = read_subject_from_json(file, "exam");
	//rush
	strcpy(file, JSON_PATH);
	strcat(file, RUSH_FILE);
	subject->rush = read_subject_from_json(file, "rush");
	subject->rush->peer = init_peers();

	return subject;
}

void *read_subject_from_json(char *sub_file, char *sub_name)
{
	JSON_Value	*rootValue;
	JSON_Object	*rootObject;
	JSON_Array	*array;
	JSON_Value	*branchVal;
	JSON_Object	*branchObj;
	t_subject	*subject;
	int			count;

	rootValue = json_parse_file(sub_file);
	rootObject = json_value_get_object(rootValue);
	array = json_object_get_array(rootObject, sub_name);
	count = json_array_get_count(array);
	subject = calloc(count, sizeof(t_subject));
	for (int i = 0; i < count; i++)
	{
		branchVal = json_array_get_value(array, i);
		branchObj = json_value_get_object(branchVal);
		subject[i].type					= (int)json_object_get_number(branchObj, "type");
		subject[i].stat.success			= (int)json_object_get_number(branchObj, "success");
		subject[i].stat.avoid			= (int)json_object_get_number(branchObj, "avoid");
		subject[i].stat.hp				= (int)json_object_get_number(branchObj, "hp");
		subject[i].stat.time			= (int)json_object_get_number(branchObj, "time");
		subject[i].stat.try_cnt			= (int)json_object_get_number(branchObj, "try_cnt");
		subject[i].stat.comprehension	= (int)json_object_get_number(branchObj, "comprehension");
		subject[i].stat.done			= (int)json_object_get_number(branchObj, "done");
		subject[i].event.title			= (char *)malloc(sizeof(char) * 1024);
		subject[i].event.content		= (char *)malloc(sizeof(char) * 1024);
		strcpy(subject[i].event.title,	json_object_get_string(branchObj, "title"));
		strcpy(subject[i].event.content,json_object_get_string(branchObj, "content"));
	}
	json_value_free(rootValue);
	return	(void *)subject;
}

void *init_peers()
{
	t_peer			*peer;
	t_event			*peer_event;
	t_peer_status	*peer_status;
	t_peer_role		*peer_role;
	char			path[20];
	char			num[10];
	char			*file;
	int				count;
	int				event_count;
	int				status_count;
	int				role_count;

	file = (char *)malloc(sizeof(char) * 20);
	peer = calloc(2, sizeof(t_peer));
	//peer_status
	strcpy(file, JSON_PATH);
	strcat(file, PEER_EVENT_FILE);
	peer_event	= read_peer_event_from_json(file, "peer_event", &event_count);
	//peer_type
	strcpy(file, JSON_PATH);
	strcat(file, PEER_ROLE_FILE);
	peer_role	= read_peer_role_from_json(file, "peer_role", &status_count);
	//peer_status
	strcpy(file, JSON_PATH);
	strcat(file, PEER_STATUS_FILE);
	peer_status	= read_peer_status_from_json(file, "peer_status", &role_count);

	srand(time(NULL));
	memmove((void *)&peer->event,	&peer_event[rand() % event_count + 1], sizeof(t_event));
	memmove((void *)&peer->role,	&peer_role[rand() % role_count + 1], sizeof(t_peer_role));
	memmove((void *)&peer->status,	&peer_status[rand() % status_count + 1], sizeof(t_peer_status));

	free(file);
	free(peer_event);
	free(peer_status);
	free(peer_role);

	return peer;
}

void *read_peer_event_from_json(char *file, char *name, int *ret_count){
	JSON_Value		*rootValue;
	JSON_Object		*rootObject;
	JSON_Array		*array;
	JSON_Value		*branchVal;
	JSON_Object		*branchObj;
	t_event			*event;
	int				count;

	rootValue = json_parse_file(file);
	rootObject = json_value_get_object(rootValue);
	array = json_object_get_array(rootObject, name);
	count = json_array_get_count(array);
	event = calloc(count, sizeof(t_event));
	for (int i = 0; i < count; i++)
	{
		branchVal = json_array_get_value(array, i);
		branchObj = json_value_get_object(branchVal);
		event[i].title		= (char *)malloc(sizeof(char) * 1024);
		event[i].content	= (char *)malloc(sizeof(char) * 1024);
		strcpy(event[i].title,  (char *)json_object_get_string(branchObj, "title"));
		strcpy(event[i].content,(char *)json_object_get_string(branchObj, "content"));
	}
	*ret_count = count;
	json_value_free(rootValue);
	return	(void *)event;
}

void *read_peer_role_from_json(char *file, char *name, int *ret_count){
	JSON_Value		*rootValue;
	JSON_Object		*rootObject;
	JSON_Array		*array;
	JSON_Value		*branchVal;
	JSON_Object		*branchObj;
	t_peer_role		*role;
	int				count;

	rootValue = json_parse_file(file);
	rootObject = json_value_get_object(rootValue);
	array = json_object_get_array(rootObject, name);
	count = json_array_get_count(array);
	role = calloc(count, sizeof(t_peer_role));
	for (int i = 0; i < count; i++)
	{
		branchVal = json_array_get_value(array, i);
		branchObj = json_value_get_object(branchVal);
		role[i].type 		  = (int)json_object_get_number(branchObj, "type");
		role[i].comprehension = (int)json_object_get_number(branchObj, "comprehension");
		role[i].comment = malloc(sizeof(char) * 1024);
		strcpy(role[i].comment, (char *)json_object_get_string(branchObj, "comment"));
		strcpy(role[i].name,(char *)json_object_get_string(branchObj, "name"));
	}
	*ret_count = count;
	json_value_free(rootValue);
	return	(void *)role;
}

void *read_peer_status_from_json(char *file, char *name, int* ret_count){
	JSON_Value		*rootValue;
	JSON_Object		*rootObject;
	JSON_Array		*array;
	JSON_Value		*branchVal;
	JSON_Object		*branchObj;
	t_peer_status	*status;
	int				count;

	rootValue = json_parse_file(file);
	rootObject = json_value_get_object(rootValue);
	array = json_object_get_array(rootObject, name);
	count = json_array_get_count(array);
	status = calloc(count, sizeof(t_peer_status));
	for (int i = 0; i < count; i++)
	{
		branchVal = json_array_get_value(array, i);
		branchObj = json_value_get_object(branchVal);
		status[i].type = (int)json_object_get_number(branchObj, "type");
		strcpy(status[i].name, (char *)json_object_get_string(branchObj, "name"));
	}
	*ret_count = count;
	json_value_free(rootValue);
	return	(void *)status;
}

void parse_status(t_status *status, JSON_Object *json){
	status->level			= (int)json_object_get_number(json, "level");
	status->exp				= (int)json_object_get_number(json, "exp");
	status->intel			= (int)json_object_get_number(json, "intel");
	status->dex				= (int)json_object_get_number(json, "dex");
	status->luck			= (int)json_object_get_number(json, "luck");
	status->mental			= (int)json_object_get_number(json, "mental");
	status->activ_point 	= (int)json_object_get_number(json, "activ_point");
	status->fame 			= (int)json_object_get_number(json, "fame");
	status->fighting_point	= (int)json_object_get_number(json, "fighting_point");
}