//
//  Key.h
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 10/6/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.


#ifndef Back_2_Basics_Key_h
#define Back_2_Basics_Key_h

#include <iostream>
#include <vector>

template <class KeyType>
class Key
{
public:
  std::vector<uint8_t> name;
  
  Key(KeyType key)
  {
    name.assign((uint8_t *)&key,(uint8_t *)&key + sizeof(KeyType));
  }
  
  Key(KeyType *key)
  {
    name.assign((uint8_t *)key,(uint8_t *)key + sizeof(key));
  }
  
  bool operator== (Key &param)
  {
    if (name.size() != param.name.size())
    {
      return false;
    }
    
    return (!memcmp(&name.at(0), &param.name.at(0), name.size()));
  }
  
  bool operator== (KeyType &param)
  {
    if (name.size() != sizeof(KeyType))
    {
      return false;
    }
    
    return (!memcmp(&name.at(0), &param, name.size()));
  }
  
  void operator= (KeyType &param)
  {
    if (name.size() != sizeof(KeyType))
    {
      delete name;
      name = new std::vector<uint8_t>((uint8_t *)&param,(uint8_t *)&param + sizeof(KeyType));
    }
    else
    {
      name.assign((uint8_t *)&param,(uint8_t *)&param + sizeof(KeyType));
    }
  }
  
};

#endif
