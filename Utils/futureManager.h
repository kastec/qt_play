#ifndef FUTUREMANAGER_H
#define FUTUREMANAGER_H


#include <QFutureWatcher>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>
#include <functional>
#include <vector>
#include <memory>
#include <optional>
#include <stdexcept>


class FutureResultBase {
  public:
    virtual ~FutureResultBase()  =default;
};    

template<typename T>
class FutureResult : public FutureResultBase {
  public:
    T value;
    explicit FutureResult(T val) : value(std::move(val)) {}
    
     ~FutureResult(){
        qDebug() << "  FResult::destr()" << value;
    };
};

class FutureResults {
  private:
    
    QVector<FutureResultBase*> results;
    
  public:
    FutureResults(){
        qDebug()<< "FutureResults::CTOR";        
    }
    
    void resize(size_t count) {
        results.resize(count);
    }

    void setResult(int index, FutureResultBase* result) {
        if (index >= results.size())
            throw std::out_of_range("FutureResult: Invalid result index");
        
        results[index] = result;
    }
    
    template<typename T>
    T getResult(int index) {
        qDebug()<< "getRes" << index;
        if (index >= results.size())
            throw std::out_of_range("FutureResult: Out of range result index");
        
        if (results[index]==nullptr)
            throw std::runtime_error("FutureResult: Result not available");
        
        qDebug()<< "getRes - cast";
        auto* typedResult = dynamic_cast<FutureResult<T>*>(results[index]);
        if (!typedResult)
            throw std::bad_cast();
        qDebug()<< "getRes - val:" << typedResult->value;
        return typedResult->value;
    }
    
    int size() const { return results.size();  }
    
    ~FutureResults()
    {
        qDebug()<< "FutureResults::DESTR";
        qDeleteAll(results);        
    };
};

class FutureManager : public QObject {
    Q_OBJECT
    
  private:
    struct FutureEntry {
        QObject* watcher;
        size_t index;
    };
    
    std::vector<FutureEntry> futures;
    FutureResults results;
    
    std::atomic<int> completedCount{0};
    std::optional<std::function<void( FutureResults*)>> completionCallback;
     
  signals:
    void allCompleted(FutureResults* results);
    
  public:
    FutureManager() = default;
    
    void setCallback(std::function<void( FutureResults*)> callback) {
        completionCallback = std::move(callback);
    }
    
    template<typename T>
    void add(QFuture<T> future) {
        auto* watcher = new QFutureWatcher<T>(this);
        size_t currentIndex = futures.size();
        
        connect(watcher, &QFutureWatcher<T>::finished, this, [this, watcher]() {            
            handleFutureCompleted<T>(watcher);
        });
        
        watcher->setFuture(future);
        futures.push_back({watcher, currentIndex});
        results.resize(futures.size());
    }
    
    template<typename Func>
    FutureManager& addTask(Func&& func) {
        auto future = QtConcurrent::run(std::forward<Func>(func));
        add(std::move(future));
        return *this;
    }
    
    FutureResults* waitAll()
    {
        QEventLoop loop;
        connect(this, &FutureManager::allCompleted, &loop, &QEventLoop::quit);
        loop.exec();
        return &results;
    }
    
  private:
    template<typename T>
    void handleFutureCompleted(QFutureWatcher<T>* watcher) {
        auto it = std::find_if(futures.begin(), futures.end(),
                               [watcher](const FutureEntry& entry) {
                                   return entry.watcher == watcher;
                               });
        
        if (it != futures.end()) {
            auto res = new FutureResult<T>(watcher->result());
           // qDebug()<<"set-val" << it->index << res->value;
            results.setResult(it->index, res);
        }
        
        completedCount++;
        if (completedCount == (int)futures.size())
        {
            if (completionCallback)
                (*completionCallback)(&results);
            emit allCompleted(&results);
        }
    }
    
  public:
    ~FutureManager() {
        qDebug() << "~FutureManager()";
  
        for (auto& entry : futures) {
//            qDebug() << "  future: " << entry.index;
            if (auto* watcher = entry.watcher) {
                if (auto* typedWatcher = dynamic_cast<QFutureWatcherBase*>(watcher)) {
                    typedWatcher->waitForFinished();
                }
//                qDebug() << "  del future: " << entry.index << watcher;
                delete watcher;
            }
        }
        
    }

};


#endif // FUTUREMANAGER_H
