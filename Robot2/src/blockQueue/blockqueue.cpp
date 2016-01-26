#include "blockqueue.h"

blockQueue::blockQueue()
{
}

pthread_mutex_t blockQueue::mutex_queue_char = PTHREAD_MUTEX_INITIALIZER;

std::queue < char > blockQueue::_charQ;
void blockQueue::_add(char X)
{
    //digitalWrite(LED7,1);  // zapalam sygnal odbioru wiadomosci

 pthread_mutex_lock(&mutex_queue_char);

_charQ.push(X);

pthread_mutex_unlock(&mutex_queue_char);

}

char blockQueue::_get( )
{  char temp;
  //  digitalWrite(LED7,0);  // zapalam sygnal odbioru wiadomosci

pthread_mutex_lock(&mutex_queue_char);

if (_charQ.size() > 0){
temp = _charQ.front();

_charQ.pop();
}
else{
  pthread_mutex_unlock(&mutex_queue_char);

return 'a';
}
pthread_mutex_unlock(&mutex_queue_char);

return temp;
}

int blockQueue::_size()
{   int tmp;
    pthread_mutex_lock(&mutex_queue_char);
 tmp = _charQ.size();
    pthread_mutex_unlock(&mutex_queue_char);
    return tmp;
}
