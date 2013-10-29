#include "dispatch.h"
//TODO: макрос на последовательное выполнение нескольких функций (несколько функций и несколько параметров)
// ввести функцию фриз (Freeze) т.е. увеличивать время задержки выбранной следующей и в данный момент выполняемой
// функции на время выполняемой добавляемой функции , которую можно назвать дочерней .
// должно получиться что то вроде ожидания выполнения дочернего процесса или передаче процесса
// TODO:Загнать счетчик длительности задачи прямо на флаг таймера , ввести прерывание функции по команде и возврат на место
// прерывания (goto ??? не красиво , но должно сработать , или return значением)
// TODO: Ввести максимальное время выполнение функции и принудительная передача управления диспетчера перехватом из задержки
// TODO: добавить в вызов функции обратный вызов функции и код таблицы набора ее переменных
// TODO: прикрутить диспетчер
// чтобы передать параметры передаваемой функции нужно указать их тип для начала
// а потом уже мутить перегрузку и шаблоны...
// в голове пока крутится обратный вызов шаблона функции , которая по перегрузке выберет нужную функцию или где то так
// другой вариант это куча костылей на каждую функцию , получится слишком громоздко , но зато для меня проще в реализации
//             || || || || || || ||
//             \/ \/ \/ \/ \/ \/ \/
void AddTask (void (*taskfunc)(void),void (*nextfunc)(void), uint16_t taskdelay,uint16_t nextdelay,u16 taskruns){
   u8 n=0;
   u8 position=0;
   while (((TaskArray[n].pfunc!=0)||(TaskArray[n].delay!=0))&&(TaskArray[n].delay<=((taskdelay==0)?(++taskdelay):(taskdelay))&&(n < MAXnTASKS)))n++;
    position=n;
   while ((TaskArray[n].pfunc != 0) && (TaskArray[n].delay!=0) && (n < MAXnTASKS))n++;
   for (/*.*/;n>position;n--){
        TaskArray[n]=TaskArray[n-1];
   }
        TaskArray[position].pfunc = taskfunc;
        TaskArray[position].delay = taskdelay;
        TaskArray[position].nextdelay = nextdelay;  // стал не нужен , все переместилось в таскранс
        TaskArray[position].run = 0;
        TaskArray[position].numRun = taskruns;
        TaskArray[position].nextfunc = nextfunc; //  вызывает следующую ф-ию с параметрами предыдущей :(
}


void DispatchTask (void){
    uint8_t n=0;
    if (flags.RunFlag==1){                     // если таймер выставил флаг
        task tmp;                       // переменная для хранения нулевого элемента
        tmp=TaskArray[0];
    while (((TaskArray[n].pfunc != 0) || (TaskArray[n].delay!=0)) && (n < MAXnTASKS)){
        n++; //мотаем пока счетчик не дойдет до задачи с нужной задержкой
        TaskArray[n-1]=TaskArray[n];        //сдвигаем очередь вперед
        if (TaskArray[n-1].delay) TaskArray[n-1].delay-=dt;     //вычитаем прошедшее время из каждой задачи
   }
    DeleteTask(n);      //удаляем последнюю задачу
    // добавление задачи
    // 1. Добавление одиночной задачи task delay
    // 2. Добавление количества запусков одиночной задачи numRun
    // 3. Добавление периодической задачи task period
    // если намран равен 0 , то это одиночная задача , если след. ф-ия идл , то не ставить в очередь.
    // если намран макс , то это зацикленная задача
    // если другое значение , то значит это количество запусков , уменьшаем его с каждой вставкой
    // косяк скорее всего с тем , что нельзя будет поменять количество запусков или задержку
    // только через функцию посредник
    switch (tmp.numRun){
            case 0: if (*tmp.nextfunc!=Idle) AddTask(*tmp.nextfunc,Idle,tmp.nextdelay,0,tmp.numRun);break;
            case 0xffff: AddTask(*tmp.pfunc,*tmp.nextfunc,tmp.delay,tmp.nextdelay,tmp.numRun);break;
            default: if (tmp.numRun&&tmp.numRun!=0xffff) AddTask(*tmp.pfunc,*tmp.nextfunc,tmp.delay,tmp.nextdelay,--tmp.numRun);break;
    }
  //  if (tmp.period==0&&tmp.numRun==0) {AddTask(*tmp.pfunc,*tmp.nextfunc,tmp.delay,tmp.period,tmp.numRun);}
   // if (tmp.period&&tmp.numRun==0) {AddTask(*tmp.pfunc,*tmp.nextfunc,tmp.period,tmp.period,tmp.numRun);}
   // if (tmp.period&&tmp.numRun!=0) {AddTask(*tmp.pfunc,*tmp.nextfunc,tmp.period,tmp.period,--tmp.numRun);}
  //  else AddTask(*tmp.pfunc,tmp.period,tmp.period);
    //TODO:разобраться с задержками запуска функций, трабла с нулевым значением, временные интервалы не точны
    if (TaskArray[0].delay!=0) {delay_time=TaskArray[0].delay;} // если здесь +1 , то немного работает ))))
    else {delay_time=1;} //можно флаг запуска добавить сюда , но в очереди будет нечего убавлять и ф-ию зациклит
    dt=delay_time;      //или воткнуть туда значение уменьшения , только его нужно брать для точности
    (*tmp.pfunc)();
    flags.RunFlag=0;       //из расчета кол-ва тиков выполняемой функции и частоты прерывания таймера
   }
}

void ResetTask(void (*resfunc)(void)){
//    task tmp;
    int i=0;
    while (TaskArray[i].pfunc!=*resfunc)i++;
    TaskArray[i].numRun=0;
}

void DeleteTask (u8 j)
{
   TaskArray[j].pfunc = 0x0000;
   TaskArray[j].delay = 0;
//   TaskArray[j].period = 0;
   TaskArray[j].run = 0;
   TaskArray[j].nextfunc = 0x0000;
   TaskArray[j].numRun =0;
   TaskArray[j].nextdelay=0;
}

void Idle(){

}
