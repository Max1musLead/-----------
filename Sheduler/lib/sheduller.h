#pragma once

#include <cmath>
#include <iostream>
#include <iterator>
#include <vector>
#include <map>
#include <memory>

template<typename T>
T getValue(T& v) {
    return v;
}

class TTaskScheduler {
private:
////////// КЛАСС ОБЪЕКТА РЕЗУЛЬТАТА В БУДУЩЕМ ///////////
    template<typename Type> 
    class FutureResult {
    private:
        Type* result;
        int taskId;
    public:
        FutureResult(Type* result_, int taskId_) : result(result_), taskId(taskId_) {}
        Type& getTypeFuture() {
            return *result;
        }
        int getId() {
            return taskId;
        }
    };

///////// Проверка на Future ///////////
    template <typename T>
    struct is_future_result : std::false_type {};
    template <typename T>
    struct is_future_result<FutureResult<T>> : std::true_type {};

///////// Задание базовый ///////////
    struct ITask {
        virtual void execute() = 0;
        virtual bool isExecuted() = 0;
        virtual void* getResult() = 0;
        virtual void* getFuture() = 0;
        virtual ~ITask() {}
    };

////////// БЕЗ АРГУМЕНТОВ ///////////
    template <typename Ret_Func, typename Func>
    class Task : public ITask {
    private:
        bool executed = false;
        Func func;
        std::shared_ptr<Ret_Func> result;

    public:
        Task(const Func& f) : func(f), result(new Ret_Func) {}

        void execute() override {
            result = std::make_shared<Ret_Func>(func());
            executed = true;
        }

        bool isExecuted() override { 
            return executed; 
        }

        void* getResult() override {
            if (!executed) {
                execute();
            }
            return static_cast<void*>(result.get());
        }

        void* getFuture() override {
            return static_cast<void*>(result.get());
        }
    };

////////// 1 АРГУМЕНТ ///////////
    template <typename Ret_Func, typename Func, typename Arg1>
    class TaskWithOneArg : public ITask {
    private:
        bool executed = false;
        bool fut_arg = false;
        Func func;
        Arg1 arg1;
        std::shared_ptr<Ret_Func> result;
        std::vector<std::shared_ptr<ITask>> predecessors;

    public:
        TaskWithOneArg(const Func& f, const Arg1& a1, const bool& fut_arg_ = 0) 
        : fut_arg(fut_arg_), func(f), arg1(a1), result(new Ret_Func) {}

        void execute() override {
            for (auto& pred : predecessors) {
                if (!pred->isExecuted()) {
                    pred->execute();
                }
                if (fut_arg) {
                    arg1 = *static_cast<Ret_Func*>(pred->getResult());
                    fut_arg = false;
                }
            }
            result = std::make_shared<Ret_Func>(func(getValue(arg1)));
            executed = true;
        }

        bool isExecuted() override { 
            return executed; 
        }

        void* getResult() override {
            if (!executed) {
                execute();
            }
            return static_cast<void*>(result.get());
        }

        void* getFuture() override {
            return static_cast<void*>(result.get());
        }

        void addPredecessor(const std::shared_ptr<ITask>& task) {
            predecessors.push_back(task);
        }
    };

/////// 2 АРГУМЕНТА /////////
    template <typename Ret_Func, typename Func, typename Arg1, typename Arg2>
    class TaskWithTwoArgs : public ITask {
    private:
        bool fut_arg1 = false, fut_arg2 = false;
        Func func;
        Arg1 arg1;
        Arg2 arg2;
        bool executed = false;
        std::shared_ptr<Ret_Func> result;
        std::vector<std::shared_ptr<ITask>> predecessors;

    public:
        TaskWithTwoArgs(const Func& f, const Arg1& a1, const Arg2& a2, const bool& fut_arg1_ = 0, const bool& fut_arg2_ = 0) 
        : func(f), arg1(a1), arg2(a2), result(new Ret_Func), fut_arg1(fut_arg1_), fut_arg2(fut_arg2_) {}
        
        void execute() override {
            for (auto& pred : predecessors) {
                if (!pred->isExecuted()) {
                    pred->execute();
                }
                if (fut_arg1) {
                    arg1 = *static_cast<Ret_Func*>(pred->getResult());
                    fut_arg1 = false;
                } else if (fut_arg2) {
                    arg2 = *static_cast<Ret_Func*>(pred->getResult());
                    fut_arg2 = false;
                }

            }
            result = std::make_shared<Ret_Func>(func(getValue(arg1), getValue(arg2)));
            executed = true;
        }

        bool isExecuted() override { 
            return executed; 
        }

        void* getResult() override {
            if (!executed) {
                execute();
            }
            return static_cast<void*>(result.get());
        }

        void* getFuture() override {
            return static_cast<void*>(result.get());
        }

        void addPredecessor(const std::shared_ptr<ITask>& task) {
            predecessors.push_back(task);
        }
    };

    std::map<int, std::shared_ptr<ITask>> tasks;
    int nextId = 0;

private:

    template<typename Func, typename Arg1, typename Arg2>
    int processFunctionTwo(Func func, Arg1 arg1, Arg2 arg2, int rel_idx1, int rel_idx2, bool fut_arg1 = 0, bool fut_arg2 = 0) {
        using Ret_Type = decltype(func(arg1, arg2));
        auto task = std::make_shared<TaskWithTwoArgs<Ret_Type, Func, Arg1, Arg2>>(func, arg1, arg2, fut_arg1, fut_arg2);

        if (rel_idx1 != -1) {
            task->addPredecessor(tasks[rel_idx1]);
        }
        if (rel_idx2 != -1) {
            task->addPredecessor(tasks[rel_idx2]);
        }
        
        tasks[nextId] = task;
        return nextId++;
    }

public:
    void executeAll() {
        for (const auto& i : tasks) {
            if (!i.second->isExecuted()) {
                i.second->execute();
            }
        }
    }

    template <typename Func>
    int add(Func func) {
        using Ret_Type = decltype(func());
        auto task = std::make_shared<Task<Ret_Type, Func>>(func);
        tasks[nextId] = task;
        return nextId++;
    }

    template <typename Func, typename Arg1>
    int add(Func func, Arg1 arg1) {
        if constexpr (is_future_result<Arg1>::value) {
            using Ret_Type = decltype(func(arg1.getTypeFuture()));
            auto arg1_ = arg1.getTypeFuture();
            auto task = std::make_shared<TaskWithOneArg<Ret_Type, Func, decltype(arg1_)>>(func, arg1.getTypeFuture(), 1);
            task->addPredecessor(tasks[arg1.getId()]);
            tasks[nextId] = task;
            return nextId++;
        } else {
            using Ret_Type = decltype(func(getValue(arg1)));
            auto task = std::make_shared<TaskWithOneArg<Ret_Type, Func, Arg1>>(func, getValue(arg1), 0);
            tasks[nextId] = task;
            return nextId++;
        }
    }

    template <typename Func, typename Arg1, typename Arg2>
    int add(Func func, Arg1 arg1, Arg2 arg2) {
        int rel_idx1 = -1;
        int rel_idx2 = -1;
        if constexpr (is_future_result<Arg1>::value) {
            auto arg1_ = arg1.getTypeFuture();
            rel_idx1 = arg1.getId();
            if constexpr (is_future_result<Arg2>::value) { 
                auto arg2_ = arg2.getTypeFuture();
                rel_idx2 = arg2.getId();
                return processFunctionTwo(func, arg1_, arg2_, rel_idx1, rel_idx2, 1, 1);
            } else {
                return processFunctionTwo(func, arg1_, getValue(arg2), rel_idx1, rel_idx2, 1);
            }
        } else {
            if constexpr (is_future_result<Arg2>::value) {
                auto arg2_ = arg2.getTypeFuture();
                rel_idx1 = arg2.getId();
                return processFunctionTwo(func, getValue(arg1), arg2_, rel_idx1, rel_idx2, 0, 1);
            } else {
                return processFunctionTwo(func, getValue(arg1), getValue(arg2), rel_idx1, rel_idx2);
            }
        }
    }

    template <typename Type>
    Type getResult(const int& idx) {
        auto it = tasks.find(idx);
        if (it == tasks.end()) {
            throw std::runtime_error("Task ID not found");
        }

        return *static_cast<Type*>(it->second->getResult());
    }

    template<typename Type>
    FutureResult<Type> getFutureResult(const int& idx) {
        auto it = tasks.find(idx);
        if (it == tasks.end()) {
            throw std::runtime_error("Task ID not found");
        }

        return FutureResult<Type>(static_cast<Type*>(it->second->getFuture()), idx);
    }
};
