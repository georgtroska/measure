// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME DictOutput
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "C:/Users/troska/Desktop/git/measure/test/A.h"
#include "C:/Users/troska/Desktop/git/measure/test/oneheader.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *Device_Dictionary();
   static void Device_TClassManip(TClass*);
   static void *new_Device(void *p = nullptr);
   static void *newArray_Device(Long_t size, void *p);
   static void delete_Device(void *p);
   static void deleteArray_Device(void *p);
   static void destruct_Device(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Device*)
   {
      ::Device *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Device));
      static ::ROOT::TGenericClassInfo 
         instance("Device", "oneheader.h", 5,
                  typeid(::Device), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Device_Dictionary, isa_proxy, 1,
                  sizeof(::Device) );
      instance.SetNew(&new_Device);
      instance.SetNewArray(&newArray_Device);
      instance.SetDelete(&delete_Device);
      instance.SetDeleteArray(&deleteArray_Device);
      instance.SetDestructor(&destruct_Device);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Device*)
   {
      return GenerateInitInstanceLocal(static_cast<::Device*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Device*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Device_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::Device*>(nullptr))->GetClass();
      Device_TClassManip(theClass);
   return theClass;
   }

   static void Device_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *A_Dictionary();
   static void A_TClassManip(TClass*);
   static void *new_A(void *p = nullptr);
   static void *newArray_A(Long_t size, void *p);
   static void delete_A(void *p);
   static void deleteArray_A(void *p);
   static void destruct_A(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::A*)
   {
      ::A *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::A));
      static ::ROOT::TGenericClassInfo 
         instance("A", "A.h", 5,
                  typeid(::A), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &A_Dictionary, isa_proxy, 1,
                  sizeof(::A) );
      instance.SetNew(&new_A);
      instance.SetNewArray(&newArray_A);
      instance.SetDelete(&delete_A);
      instance.SetDeleteArray(&deleteArray_A);
      instance.SetDestructor(&destruct_A);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::A*)
   {
      return GenerateInitInstanceLocal(static_cast<::A*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::A*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *A_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::A*>(nullptr))->GetClass();
      A_TClassManip(theClass);
   return theClass;
   }

   static void A_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_Device(void *p) {
      return  p ? new(p) ::Device : new ::Device;
   }
   static void *newArray_Device(Long_t nElements, void *p) {
      return p ? new(p) ::Device[nElements] : new ::Device[nElements];
   }
   // Wrapper around operator delete
   static void delete_Device(void *p) {
      delete (static_cast<::Device*>(p));
   }
   static void deleteArray_Device(void *p) {
      delete [] (static_cast<::Device*>(p));
   }
   static void destruct_Device(void *p) {
      typedef ::Device current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Device

namespace ROOT {
   // Wrappers around operator new
   static void *new_A(void *p) {
      return  p ? new(p) ::A : new ::A;
   }
   static void *newArray_A(Long_t nElements, void *p) {
      return p ? new(p) ::A[nElements] : new ::A[nElements];
   }
   // Wrapper around operator delete
   static void delete_A(void *p) {
      delete (static_cast<::A*>(p));
   }
   static void deleteArray_A(void *p) {
      delete [] (static_cast<::A*>(p));
   }
   static void destruct_A(void *p) {
      typedef ::A current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::A

namespace {
  void TriggerDictionaryInitialization_libDictOutput_Impl() {
    static const char* headers[] = {
"C:/Users/troska/Desktop/git/measure/test/A.h",
"C:/Users/troska/Desktop/git/measure/test/oneheader.h",
nullptr
    };
    static const char* includePaths[] = {
"C:/root_v6.28.06/include",
"C:/Users/troska/Desktop/git/measure/test",
"C:/root_v6.28.06/include/",
"C:/Users/troska/Desktop/git/measure/test/build/",
nullptr
    };
    static const char* fwdDeclCode = nullptr;
    static const char* payloadCode = nullptr;
    static const char* classesHeaders[] = {
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libDictOutput",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libDictOutput_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libDictOutput_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libDictOutput() {
  TriggerDictionaryInitialization_libDictOutput_Impl();
}
