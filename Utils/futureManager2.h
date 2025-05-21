
#ifndef FUTUREMANAGER2_H
#define FUTUREMANAGER2_H

#include <QObject>
#include <QFutureWatcher>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>
#include <functional>
#include <vector>
#include <memory>
#include <optional>
#include <stdexcept>

class FutureManager2Base : public QObject {
    Q_OBJECT
    
  signals:
    void allTasksCompleted();
  public:
    FutureManager2Base() : QObject() {}    
};

template<typename T1, typename T2>
class FutureManager2 : public FutureManager2Base{
       
  private:
    QFutureWatcher<T1>* watcher1;
    QFutureWatcher<T2>* watcher2;
    std::atomic<int> completedTasks{0};
    
    T1 result1;
    T2 result2;
    
  public:
    FutureManager2() : completedTasks(0) {
        // Инициализация QFutureWatcher
        watcher1 = new QFutureWatcher<T1>(this);
        watcher2 = new QFutureWatcher<T2>(this);
        
               // Подключаем сигналы завершения
        connect(watcher1, &QFutureWatcher<T1>::finished, this, &FutureManager2::onTask1Finished);
        connect(watcher2, &QFutureWatcher<T2>::finished, this, &FutureManager2::onTask2Finished);
    }
    
    std::pair<T1, T2> startTasks(std::function<T1()> task1, std::function<T2()> task2) {
        // Запускаем первую задачу
        QFuture<T1> future1 = QtConcurrent::run(task1);
        watcher1->setFuture(future1);
        
               // Запускаем вторую задачу
        QFuture<T2> future2 = QtConcurrent::run(task2);
        watcher2->setFuture(future2);
        
        QEventLoop loop;
        connect(this, &FutureManager2<T1,T2>::allTasksCompleted, &loop, &QEventLoop::quit);
        loop.exec();
        
        return std::make_pair(result1, result2);
    }

    void onTask1Finished() {
        completedTasks++;
        this->result1 = watcher1->result();
        //        qDebug() << "Задача 1 завершена. Результат:" << result;
        checkAllTasksCompleted();
    }
    
    void onTask2Finished() {
        completedTasks++;
        this->result2 = watcher2->result();
        //        qDebug() << "Задача 2 завершена. Результат:" << result;
        checkAllTasksCompleted();
    }
    
    
  private:
    void checkAllTasksCompleted() {
        if (completedTasks == 2) {
            //            qDebug() << "Все задачи завершены!";
            emit allTasksCompleted();
        }
    }
    
    
  public:
    ~FutureManager2() {
        // Ждём завершения всех операций перед удалением
        watcher1->waitForFinished();
        watcher2->waitForFinished();
        delete watcher1;
        delete watcher2;
    }
};


#endif // FUTUREMANAGER2_H
