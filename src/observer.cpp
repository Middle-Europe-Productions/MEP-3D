#include<MEP-3D/observer.hpp>

int Observer::id_couter_ = 0;

Observer::Observer() : id_(id_couter_++) {}

bool Observer::IsEqual(Observer* obs) const {
    return obs && id_ == obs->id_;
}

bool Observer::GetObsserverId() const {
    return id_;
}