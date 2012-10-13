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
#include <vector>

template <class KeyType>
class Key {
public:
  std::vector<uint8_t> name;
  
  Key(KeyType key)  {
    name.assign((uint8_t *)&key,(uint8_t *)&key + sizeof(KeyType));
  }
  
  Key(KeyType *key)  {
    name = new std::vector<uint8_t>((uint8_t *)key,(uint8_t *)key + sizeof(key));
  }
  
  bool operator== (Key &param) {
    if (name.size() != param.name.size()) {
      return false;
    }
    
    return (!memcmp(&name.at(0), &param.name.at(0), name.size()));
  }
  
  bool operator== (KeyType &param) {
    if (name.size() != sizeof(KeyType)) {
      return false;
    }
    
    return (!memcmp(&name.at(0), &param, name.size()));
  }
  
  void operator= (KeyType &param) {
    if (name.size() != sizeof(KeyType)) {
      delete name;
      name = new std::vector<uint8_t>((uint8_t *)&param,(uint8_t *)&param + sizeof(KeyType));
    } else {
      name.assign((uint8_t *)&param,(uint8_t *)&param + sizeof(KeyType));
    }
  }
  
};

#endif
