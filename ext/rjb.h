/*
 * Rjb - Ruby <-> Java Bridge
 * Copyright(c) 2004,2005 arton
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * $Id$
 * $Log: rjb.h,v $
 * Revision 1.1  2005/01/16 17:36:10  arton
 * Initial revision
 *
 *
 */

#ifndef RJB_H
#define RJB_H

#if !defined(COUNTOF)
#define COUNTOF(x) (sizeof(x)/sizeof(x[0]))
#endif

#if !defined(_I64_MIN)
#define _I64_MIN    (-9223372036854775807i64 - 1)
#endif
#if !defined(_I64_MAX)
#define _I64_MAX      9223372036854775807i64
#endif

/* in rjb.c */
extern VALUE loaded_classes;
extern jmethodID class_getName;
extern jclass j_throwable;
extern jmethodID throwable_getMessage;
extern JNIEnv* attach_current_thread(void);
extern jclass find_class(JNIEnv* jenv, VALUE name);
extern void release_string(JNIEnv *jenv, jstring str, const char* chrs);

/* in rjbexception.c */
extern VALUE get_exception_class(JNIEnv* jenv, jstring str);
extern void check_exception(JNIEnv* jenv, int t);
extern VALUE rjb_s_throw(int, VALUE*, VALUE);

/* conversion functions */
typedef void (*R2J)(JNIEnv*, VALUE, jvalue*, const char*, int);
typedef VALUE (*J2R)(JNIEnv*, jvalue);
typedef jarray (*R2JARRAY)(JNIEnv*, VALUE, const char*);
typedef void (JNICALL *RELEASEARRAY)(JNIEnv*, jobject, void*, jint);
typedef jlong (JNICALL *INVOKEAL)(JNIEnv*, jobject, jmethodID, const jvalue*);
typedef jdouble (JNICALL *INVOKEAD)(JNIEnv*, jobject, jmethodID, const jvalue*);
typedef jfloat (JNICALL *INVOKEAF)(JNIEnv*, jobject, jmethodID, const jvalue*);
typedef jboolean (JNICALL *INVOKEAZ)(JNIEnv*, jobject, jmethodID, const jvalue*);
typedef jshort (JNICALL *INVOKEAS)(JNIEnv*, jobject, jmethodID, const jvalue*);
typedef jobject (JNICALL *INVOKEA)(JNIEnv*, jobject, jmethodID, const jvalue*);
typedef VALUE (*CONV)(JNIEnv*, void*);

/*
 * internal method class
 */
struct cls_constructor {
    jmethodID id;
    int arg_count;
    R2J* arg_convert;
    char* method_signature;
    char  result_signature;
    char  result_arraydepth;
};

struct cls_method {
    struct cls_constructor basic;
    ID name;
    int static_method;
    off_t method;
    J2R result_convert;
    /* overload only */
    struct cls_method* next;
};

/*
 * internal field class
 */
struct cls_field {
    ID name;
    jfieldID id;
    char* field_signature;
    char  result_signature;
    char  result_arraydepth;
    R2J arg_convert;
    J2R value_convert;
    int readonly;
    int static_field;
};

/*
 * Object instance
 */
struct jvi_data {
    jclass klass; /* class */
    jobject obj; /* instance */
    st_table* methods;
    st_table* fields;
};

/*
 * Class instance
 */
struct jv_data {
    struct jvi_data idata;
    st_table* static_methods;
    struct cls_constructor** constructors;
};

/*
 * Bridge instance
 */
struct rj_bridge {
    jobject bridge;
    jobject proxy;
    VALUE wrapped;
};

#endif