#ifndef OBSERVER_HPP
#define OBSERVER_HPP

class Observer {
 public:
  Observer();
  bool IsEqual(Observer* obs) const;
  bool GetObsserverId() const;

 private:
  static int id_couter_;
  int id_;
};

#endif