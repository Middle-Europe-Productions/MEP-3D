#ifndef OBSERVABLE_VARIABLE_HPP
#define OBSERVABLE_VARIABLE_HPP

#include <MEP-3D/observer_list.hpp>
#include <unordered_map>

template<typename Type, typename Identity>
class VariablesObserver {
public:
    virtual void OnVariableChanged(const Type& value, const Identity& identity) = 0;
    virtual ~VariablesObserver() = default;
};

template<typename Type, typename Identity>
class ObservableVariables: public ObserverList<VariablesObserver<Type, Identity>> {
public:
    const Type& Get(const Identity& identity) const {
        //static_assert(variable_map_.find(identity) != variable_map_.end());
        return variable_map_.at(identity);
    }
    void Set(const Identity& identity, const Type& new_type_) {
        if (variable_map_.find(identity) != variable_map_.end() && variable_map_[identity] == new_type_)
            return;
        variable_map_[identity] = new_type_;
        ObserverList<VariablesObserver<Type, Identity>>::ForAllObservers([&new_type_, &identity](VariablesObserver<Type, Identity>* observer){
            observer->OnVariableChanged(new_type_, identity);
        });
    }
    void Increment(const Identity& identity, const Type& value) {
        //static_assert(variable_map_.find(identity) != variable_map_.end());
        variable_map_[identity] += value;
        ObserverList<VariablesObserver<Type, Identity>>::ForAllObservers([this, &identity](VariablesObserver<Type, Identity>* observer){
            observer->OnVariableChanged(variable_map_.at(identity), identity);
        });
    }
    void Decrement(const Identity& identity, const Type& value) {
        //static_assert(variable_map_.find(identity) != variable_map_.end());
        variable_map_[identity] -= value;
        ObserverList<VariablesObserver<Type, Identity>>::ForAllObservers([this, &identity](VariablesObserver<Type, Identity>* observer){
            observer->OnVariableChanged(variable_map_.at(identity), identity);
        });
    }
private:
    std::unordered_map<Identity, Type> variable_map_;
};

#endif