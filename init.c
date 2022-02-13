#include "rpg42.h"

#define _CRT_SECURE_NO_WARNINGS // fopen 보안 경고로 인한 컴파일 에러 방지

void parse_status(t_status *status, JSON_Object *json);
t_event_day *parse_event(t_event_day *event, JSON_Object *json);
void *read_subject_from_json(char *sub_file, char *sub_name);
void *read_peer_event_from_json(char *sub_file, char *sub_name, int *count);
void *read_peer_role_from_json(char *sub_file, char *sub_name, int *count);
void *read_peer_status_from_json(char *sub_file, char *sub_name, int *count);
void read_status_from_object(void *ptr, JSON_Object *o_status);
void read_substatus_from_object(void *ptr, JSON_Object *o_sub_status);
void read_event_from_object(void *ptr, JSON_Object *event);
void read_reward_from_object(void *ptr, JSON_Object *reward);
void shuffle(t_event_day* arr, int size);
void *init_peers();

const char *EXAM_FILE = "exam.json";
const char *RUSH_FILE = "rush.json";
const char *PERSONAL_FILE = "personal.json";
const char *PEER_EVENT_FILE = "peer_event.json";
const char *PEER_STATUS_FILE = "peer_status.json";
const char *PEER_ROLE_FILE = "peer_role.json";

t_user *init_user(char type)
{
	t_user *user;
	t_status *status;
	int error = 0;

	user = (t_user *)malloc(sizeof(t_user));
	status = (t_status *)malloc(sizeof(t_status));
	user->status = status;
	print_header();
	printf("       유저의 이름을 입력해 주세요(최대 20자) : ");
	scanf("%s", user->user_name);
	print_footer();
	user->status->level = 1;
	user->status->exp = 0;
	// 능력치 기본 10 + a(전공자)
	if (type == 'a')
	{
		user->type = 전공자;
		user->status->intel = 15;
		user->status->dex = 5;
		user->status->luck = 10;
		user->status->mental = 0;
		user->status->activ_point = 10;
		user->status->fame = 0;
		user->status->fighting_point = 0;
	}
	else if (type == 'b')
	{
		user->type = 비전공자;
		user->status->intel = 1;
		user->status->dex = 1;
		user->status->luck = 10;
		user->status->mental = 0;
		user->status->activ_point = 10;
		user->status->fame = 0;
		user->status->fighting_point = 0;
	}
	user->sub_list = init_subjet_list();
	return user;
}

t_subject_list *init_subjet_list()
{
	t_subject_list *subject;
	char path[20];
	char num[10];
	char *file;

	file = (char *)malloc(sizeof(char) * 20);
	subject = (t_subject_list *)calloc(1, sizeof(t_subject_list));
	// pseronal
	strcpy(file, JSON_PATH);
	strcat(file, PERSONAL_FILE);
	subject->personal = read_subject_from_json(file, "personal");
	// exam
	strcpy(file, JSON_PATH);
	strcat(file, EXAM_FILE);
	subject->exam = read_subject_from_json(file, "exam");
	// rush
	strcpy(file, JSON_PATH);
	strcat(file, RUSH_FILE);
	subject->rush = read_subject_from_json(file, "rush");
	subject->rush->peer = init_peers();

	return subject;
}

t_event_day *init_event_day()
{
	t_event_day *day;
	JSON_Value	*rootValue;
	JSON_Object *rootObject;
	JSON_Object *tmpObj;
	char 		path[20];
	char 		num[10];
	char 		*file;

	day = (t_event_day *)malloc(sizeof(t_event_day) * EVENT_DAY_MAX);
	file = (char *)malloc(sizeof(char) * 20);

	for (int i = 0; i < EVENT_DAY_MAX; i++)
	{
		sprintf(num, "%d", i + 1);
		strcpy(file, EVENT_DAY_PATH);
		strcat(file, num);
		strcat(file, ".json");
		rootValue = json_parse_file(file);
		rootObject = json_value_get_object(rootValue);
		/* day 값 설정 */
		tmpObj = json_object_get_object(rootObject, "event");
		read_event_from_object(&day[i].event, tmpObj);
		tmpObj = json_object_get_object(rootObject, "reward");
		read_reward_from_object(&day[i].reward, tmpObj);
		json_value_free(rootValue);
	}
	free(file);
	shuffle((day + 1), EVENT_DAY_MAX - 1);
	for (int i = 0; i <EVENT_DAY_MAX; i++){
		day[i].day = i + 1;
	}
	return day;
}


void *read_subject_from_json(char *sub_file, char *sub_name)
{
	JSON_Value *rootValue;
	JSON_Object *rootObject;
	JSON_Array *array;
	JSON_Value *branchVal;
	JSON_Object *branchObj;
	JSON_Object *tmpObj;
	t_subject *subject;
	int count;

	rootValue = json_parse_file(sub_file);
	rootObject = json_value_get_object(rootValue);
	array = json_object_get_array(rootObject, sub_name);
	count = json_array_get_count(array);
	subject = malloc(sizeof(t_subject) * count);
	for (int i = 0; i < count; i++)
	{
		branchVal = json_array_get_value(array, i);
		branchObj = json_value_get_object(branchVal);

		tmpObj = json_object_get_object(branchObj, "status");
		read_substatus_from_object(&subject[i].stat, tmpObj);

		tmpObj = json_object_get_object(branchObj, "event");
		read_event_from_object(&subject[i].event, tmpObj);

		tmpObj = json_object_get_object(branchObj, "reward");
		read_reward_from_object(&subject[i].reward, tmpObj);
	}
	json_value_free(rootValue);
	return (void *)subject;
}

void *init_peers()
{
	t_peer *peer;
	t_event *peer_event;
	t_peer_status *peer_status;
	t_peer_role *peer_role;
	char path[20];
	char num[10];
	char *file;
	int count;
	int event_count;
	int status_count;
	int role_count;

	file = (char *)malloc(sizeof(char) * 20);
	peer = calloc(2, sizeof(t_peer));
	// peer_status
	strcpy(file, JSON_PATH);
	strcat(file, PEER_EVENT_FILE);
	peer_event = read_peer_event_from_json(file, "peer_event", &event_count);
	// peer_type
	strcpy(file, JSON_PATH);
	strcat(file, PEER_ROLE_FILE);
	peer_role = read_peer_role_from_json(file, "peer_role", &status_count);
	// peer_status
	strcpy(file, JSON_PATH);
	strcat(file, PEER_STATUS_FILE);
	peer_status = read_peer_status_from_json(file, "peer_status", &role_count);

	srand(time(NULL));
	memmove((void *)&peer->event, &peer_event[rand() % event_count], sizeof(t_event));
	memmove((void *)&peer->role, &peer_role[rand() % role_count], sizeof(t_peer_role));
	memmove((void *)&peer->status, &peer_status[rand() % status_count], sizeof(t_peer_status));

	free(file);
	free(peer_event);
	free(peer_status);
	free(peer_role);

	return peer;
}

void *read_peer_event_from_json(char *file, char *name, int *ret_count)
{
	JSON_Value *rootValue;
	JSON_Object *rootObject;
	JSON_Array *array;
	JSON_Value *branchVal;
	JSON_Object *branchObj;
	JSON_Object *tmpObj;
	t_event *event;
	int count;

	rootValue = json_parse_file(file);
	rootObject = json_value_get_object(rootValue);
	array = json_object_get_array(rootObject, name);
	count = json_array_get_count(array);
	event = calloc(count, sizeof(t_event));
	for (int i = 0; i < count; i++)
	{
		branchVal = json_array_get_value(array, i);
		branchObj = json_value_get_object(branchVal);
		tmpObj = json_object_get_object(branchObj, "event");
		read_event_from_object(&event[i], tmpObj);
	}
	*ret_count = count;
	json_value_free(rootValue);
	return (void *)event;
}

void *read_peer_role_from_json(char *file, char *name, int *ret_count)
{
	JSON_Value *rootValue;
	JSON_Object *rootObject;
	JSON_Array *array;
	JSON_Value *branchVal;
	JSON_Object *branchObj;
	t_peer_role *role;
	int count;

	rootValue = json_parse_file(file);
	rootObject = json_value_get_object(rootValue);
	array = json_object_get_array(rootObject, name);
	count = json_array_get_count(array);
	role = calloc(count, sizeof(t_peer_role));
	for (int i = 0; i < count; i++)
	{
		branchVal = json_array_get_value(array, i);
		branchObj = json_value_get_object(branchVal);
		role[i].type = (int)json_object_get_number(branchObj, "type");
		role[i].comprehension = (int)json_object_get_number(branchObj, "comprehension");
		role[i].comment = malloc(sizeof(char) * 1024);
		strcpy(role[i].comment, (char *)json_object_get_string(branchObj, "comment"));
		strcpy(role[i].name, (char *)json_object_get_string(branchObj, "name"));
	}
	*ret_count = count;
	json_value_free(rootValue);
	return (void *)role;
}

void *read_peer_status_from_json(char *file, char *name, int *ret_count)
{
	JSON_Value *rootValue;
	JSON_Object *rootObject;
	JSON_Array *array;
	JSON_Value *branchVal;
	JSON_Object *branchObj;
	t_peer_status *status;
	int count;

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
	return (void *)status;
}

void read_status_from_object(void *ptr, JSON_Object *o_status)
{
	t_status *status = (t_status *)ptr;
	if (status != NULL)
	{
		status->level = (int)json_object_get_number(o_status, "level");
		status->exp = (int)json_object_get_number(o_status, "exp");
		status->intel = (int)json_object_get_number(o_status, "intel");
		status->dex = (int)json_object_get_number(o_status, "dex");
		status->luck = (int)json_object_get_number(o_status, "luck");
		status->mental = (int)json_object_get_number(o_status, "mental");
		status->activ_point = (int)json_object_get_number(o_status, "activ_point");
		status->fame = (int)json_object_get_number(o_status, "fame");
		status->fighting_point = (int)json_object_get_number(o_status, "fighting_point");
	}
}

void read_reward_from_object(void *ptr, JSON_Object *o_reward)
{
	t_reward *reward = (t_reward *)ptr;
	JSON_Object *o_status = json_object_get_object(o_reward, "status");
	JSON_Object *o_equip = json_object_get_object(o_reward, "equip");
	if (reward != NULL)
	{
		reward->stat.level = (int)json_object_get_number(o_status, "level");
		reward->stat.exp = (int)json_object_get_number(o_status, "exp");
		reward->stat.intel = (int)json_object_get_number(o_status, "intel");
		reward->stat.dex = (int)json_object_get_number(o_status, "dex");
		reward->stat.luck = (int)json_object_get_number(o_status, "luck");
		reward->stat.mental = (int)json_object_get_number(o_status, "mental");
		reward->stat.activ_point = (int)json_object_get_number(o_status, "activ_point");
		reward->stat.fame = (int)json_object_get_number(o_status, "fame");
		reward->stat.fighting_point = (int)json_object_get_number(o_status, "fighting_point");
	}
}

void read_event_from_object(void *ptr, JSON_Object *o_event)
{
	t_event *event = (t_event *)ptr;
	if (event != NULL)
	{
		event->title = (char *)malloc(sizeof(char) * 1024);
		event->content = (char *)malloc(sizeof(char) * 1024);
		strcpy(event->title, json_object_get_string(o_event, "title"));
		strcpy(event->content, json_object_get_string(o_event, "content"));
	}
}

void read_substatus_from_object(void *ptr, JSON_Object *o_sub_status)
{
	t_sub_status *sub_status = (t_sub_status *)ptr;
	if (sub_status != NULL)
	{
		sub_status->percent = 0;
		sub_status->done = 0;
		sub_status->success = (int)json_object_get_number(o_sub_status, "comprehension");
		sub_status->success = (int)json_object_get_number(o_sub_status, "success");
		sub_status->hp = (int)json_object_get_number(o_sub_status, "hp");
		sub_status->avoid = (int)json_object_get_number(o_sub_status, "avoid");
		sub_status->try_cnt = (int)json_object_get_number(o_sub_status, "try_cnt");
		sub_status->time = (int)json_object_get_number(o_sub_status, "time");
	}
}


void shuffle(t_event_day* days, int size){ 
   srand(time(NULL));
   t_event_day temp;
   int rn;

   for(int i = 0; i < size; i++){ 
         rn = (rand() % (size - i)) + i;
		 temp = days[i]; 
		 days[i] = days[rn];
		 days[rn] = temp;
   }
}
