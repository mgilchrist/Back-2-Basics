//
//  Key.h
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 10/6/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#ifndef Back_2_Basics_Key_h
#define Back_2_Basics_Key_h

#include <iostream>

template <class KeyType>
class Key {
public:
  uint8_t *name;
  uint64_t  size;
  
  Key(KeyType key)  {
    size = sizeof(key);
    name = new uint8_t[size];
    bcopy(&key, name, size);
  }
  
  Key(KeyType *key)  {
    size = sizeof(key);
    name = new uint8_t[size];
    bcopy(key, name, size);
  }
  
  bool operator== (Key param) {
    if (size != param.size) {
      return false;
    }
    
    return (!bcmp(name, param.name, size));
  }
  
  bool operator== (Key *param) {
    if (size != param->size) {
      return false;
    }
    
    return (!bcmp(name, param->name, size));
  }
  
  bool operator== (KeyType *param) {
    if (size != sizeof(param)) {
      return false;
    }
    
    return (!bcmp(name, param, size));
  }
  
  bool operator== (KeyType param) {
    if (size != sizeof(param)) {
      return false;
    }
    
    return (!bcmp(name, &param, size));
  }
  
  void operator= (KeyType *param) {
    size = sizeof(param);
    name = new uint8_t[size];
    bcopy(param, name, size);
  }
  
  void operator= (KeyType param) {
    size = sizeof(param);
    name = new uint8_t[size];
    bcopy(&param, name, size);
  }
  
};

#endif
