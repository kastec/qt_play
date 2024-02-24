#include "person.h"

Person::Person(QObject *parent) : QObject{parent} {}

bool Person::getEnabled() const { return enabled; }

void Person::setEnabled(bool isEnabled) {
  if (enabled == isEnabled) {
    return;
  }

  enabled = isEnabled;

  emit enabledChanged(isEnabled);
}
