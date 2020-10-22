#include "thread_pool.h"



void handler(void *arg)
{
	pthread_mutex_unlock((pthread_mutex_t *)arg);//����
}

void *routine(void *arg)//����ִ�к���
{
	#ifdef DEBUG
	printf("[%u] is started.\n",
		(unsigned)pthread_self());
	#endif

	thread_pool *pool = (thread_pool *)arg;//������ʵ�Ǹ��̳߳�ͷ���
	struct task *p=NULL;

	while(1)
	{
		/*
		** push a cleanup function handler(), make sure that
		** the calling thread will release the mutex properly
		** even if it is cancelled during holding the mutex.
		**
		** NOTE:
		** pthread_cleanup_push() is a macro which includes a
		** loop in it, so if the specified field of codes that 
		** paired within pthread_cleanup_push() and pthread_
		** cleanup_pop() use 'break' may NOT break out of the
		** truely loop but break out of these two macros.
		** see line 56 below.
		*/
		//================================================//
		pthread_cleanup_push(handler, (void *)&pool->lock);//ע��һ���߳�������������ֹ����
		pthread_mutex_lock(&pool->lock);
		//================================================//

		// 1, no task, and is NOT shutting down, then wait
		while(pool->waiting_tasks == 0 && !pool->shutdown)
		{
			pthread_cond_wait(&pool->cond, &pool->lock);//û�����̳߳�û�йص�ʱ���������������ѵĵط�������
														//һ��������������ʱ����һ�����������̳߳�ʱ
		}

		// 2, no task, and is shutting down, then exit
		if(pool->waiting_tasks == 0 && pool->shutdown == true) //����û��������ر��̵߳Ļ� �ͽ����˳��߳�
		{
			pthread_mutex_unlock(&pool->lock);
			pthread_exit(NULL); // CANNOT use 'break';
		}

		// 3, have some task, then consume it
		p = pool->task_list->next;//��һ�������㲢û�и�ֵ�����Կ���������
		pool->task_list->next = p->next;//����pool->task_list->next
		pool->waiting_tasks--;

		//================================================//
		pthread_mutex_unlock(&pool->lock);
		pthread_cleanup_pop(0);
		//================================================//

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); //
		(p->do_task)(p->arg);//ִ������  (�����ص�)
		//func(arg);
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

		free(p);//�ͷŵ��Ա�ִ�е����� ���Ŀռ�
	}

	pthread_exit(NULL);
}

/*
	init_pool:��ʼ��һ���̳߳�
	@pool: ָ�롣ָ��Ҫ��ʼ�����̳߳�
	@threads_number:�߳��ﳣפ�߳���
*/
bool init_pool(thread_pool *pool, unsigned int threads_number)//��ʼ���̳߳���������һ�������߳��������̳߳ع�������
{
	//��ʼ��������������
	pthread_mutex_init(&pool->lock, NULL);
	pthread_cond_init(&pool->cond, NULL);

	pool->shutdown = false;//����
	pool->task_list = malloc(sizeof(struct task));//������һ�������㲢��task_listָ������ע���һ�������㲢û�и�ֵ��why?
	pool->tids = malloc(sizeof(pthread_t) * MAX_ACTIVE_THREADS);  //�̳߳�����߳��� 20  

	if(pool->task_list == NULL || pool->tids == NULL)
	{
		perror("allocate memory error");
		return false;
	}

	//ͷ�ڵ�ָ���
	pool->task_list->next = NULL;
    //��������߳���
	pool->max_waiting_tasks = MAX_WAITING_TASKS;  //���������ӵ�����Ϊ 1000 
	pool->waiting_tasks = 0; //����Ϊ0
	pool->active_threads = threads_number;

	int i;
	for(i=0; i < pool->active_threads; i++)   //����active_threads���߳�����������ִ�к���
	{
		if(pthread_create(&((pool->tids)[i]), NULL,
					routine, (void *)pool) != 0)  //�������̳߳ش��ݸ� routine
		{
			perror("create threads error");
			return false;
		}

		#ifdef DEBUG

		printf("[%u]:[%s] ==> tids[%d]: [%u] is created.\n",
			(unsigned)pthread_self(), __FUNCTION__,
			i, (unsigned)pool->tids[i]);
		#endif
	}

	return true;
}
			//��Ҫ����������̳߳�    ���ӵ�����   			  ������Ĳ���
bool add_task(thread_pool *pool,void *(*do_task)(void *arg), void *arg)//��������
{
	struct task *new_task = malloc(sizeof(struct task));//����һ���µ�������
	if(new_task == NULL)
	{
		perror("allocate memory error");
		return false;
	}
	new_task->do_task = do_task;  //���µ������㸳ֵvoid *  func(void *arg)
	new_task->arg = arg;  //������Ĳ���
	new_task->next = NULL;

	//============ LOCK =============//
	pthread_mutex_lock(&pool->lock);
	//===============================//

	if(pool->waiting_tasks >= MAX_WAITING_TASKS)//����ȴ����������ﵽ����
	{
		pthread_mutex_unlock(&pool->lock);

		fprintf(stderr, "too many tasks.\n");
		free(new_task);

		return false;//�������ӣ���������
	}
	
	struct task *tmp = pool->task_list;
	while(tmp->next != NULL)
		tmp = tmp->next;//�ҵ����һ�����

	tmp->next = new_task;
	pool->waiting_tasks++;

	//=========== UNLOCK ============//
	pthread_mutex_unlock(&pool->lock);
	//===============================//

	#ifdef DEBUG
	printf("[%u][%s] ==> a new task has been added.\n",
		(unsigned)pthread_self(), __FUNCTION__);
	#endif

	pthread_cond_signal(&pool->cond);//������������
	return true;
}

int add_thread(thread_pool *pool, unsigned additional_threads)//������߳�
{
	if(additional_threads == 0)
		return 0;

	unsigned total_threads =
			pool->active_threads + additional_threads;

	int i, actual_increment = 0;
	for(i = pool->active_threads;
	    i < total_threads && i < MAX_ACTIVE_THREADS;
	    i++)
	{
		if(pthread_create(&((pool->tids)[i]),
					NULL, routine, (void *)pool) != 0)//�����߳�
		{
			perror("add threads error");

			// no threads has been created, return fail
			if(actual_increment == 0)
				return -1;

			break;
		}
		actual_increment++; 

		#ifdef DEBUG
		printf("[%u]:[%s] ==> tids[%d]: [%u] is created.\n",
			(unsigned)pthread_self(), __FUNCTION__,
			i, (unsigned)pool->tids[i]);
		#endif
	}

	pool->active_threads += actual_increment;
	return actual_increment;
}

int remove_thread(thread_pool *pool, unsigned int removing_threads)
{ 
	if(removing_threads == 0)  //removing_threads ��Ҫɾ��������
		return pool->active_threads;

	int remaining_threads = pool->active_threads - removing_threads;

	remaining_threads = remaining_threads > 0 ? remaining_threads : 1;//ΪʲôҪ��1��why?   ������һ���߳�ȥ��������
	//remaining_threads ɾ����ʣ�µ��߳�
	int i;
	for(i = pool->active_threads - 1; i > remaining_threads - 1; i--)//���ǵ�Ŀ�ľͻ���Ӳ����Դ���Ŷ����ǽ�������
	{
		errno = pthread_cancel(pool->tids[i]);

		if(errno != 0)
			break;

		#ifdef DEBUG
		printf("[%u]:[%s] ==> cancelling tids[%d]: [%u]...\n",
			(unsigned)pthread_self(), __FUNCTION__,
			i, (unsigned)pool->tids[i]);
		#endif
	}
	//ȡ��ʧ�ܣ�I��ֵû��
	if(i == pool->active_threads-1)
		return -1;
	else
	{
		pool->active_threads = i+1;  //��ʣ�µ��߳�������ֵ��  ��Ծ��   
		return i+1;
	}
}

bool destroy_pool(thread_pool *pool)
{
	// 1, activate all threads
	pool->shutdown = true;
	pthread_cond_broadcast(&pool->cond);

	// 2, wait for their exiting
	int i;
	for(i=0; i < pool->active_threads; i++)
	{
		errno = pthread_join(pool->tids[i], NULL);
		if(errno != 0)
		{
			printf("join tids[%d] error: %s\n",
					i, strerror(errno));
		}
	}

	// 3, free memories
	free(pool->task_list);
	free(pool->tids);
	free(pool);

	return true;
}