/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   trim.h
 * Author: nyrmburk
 *
 * Created on June 23, 2017, 10:43 PM
 */

#ifndef TRIM_H
#define TRIM_H

#include <stdlib.h>
#include "../helper/publisher.h"

class trim: public publisher {
public:
    trim(size_t size);
    size_t size();
    signed short getTrim(size_t i);
    void setTrim(size_t i, signed short offset);
    virtual ~trim();
private:
    size_t m_size;
    signed short* m_trim;
};

#endif /* TRIM_H */

