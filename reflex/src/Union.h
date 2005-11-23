// @(#)root/reflex:$Name:  $:$Id: Union.h,v 1.3 2005/11/11 07:18:06 roiser Exp $
// Author: Stefan Roiser 2004

// Copyright CERN, CH-1211 Geneva 23, 2004-2005, All rights reserved.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.

#ifndef ROOT_Reflex_Union
#define ROOT_Reflex_Union

// Include Files
#include "Reflex/TypeBase.h"
#include "Reflex/ScopeBase.h"

namespace ROOT {
   namespace Reflex {

    
      /**
       * @class Union Union.h Reflex/Union.h
       * @author Stefan Roiser
       * @date 24/11/2003
       * @ingroup Ref
       */
      class Union : public TypeBase, public ScopeBase {

      public:

         /** constructor */
         Union( const char * unionType,
                size_t size,
                const std::type_info & ti ) ;


         /** destructor */
         virtual ~Union();


         /**
          * AddDataMember will add the information about a data MemberAt
          * @param dm pointer to data MemberAt
          */
         virtual void AddDataMember( const Member & dm ) const;
         virtual void AddDataMember( const char * nam,
                                     const Type & typ,
                                     size_t offs,
                                     unsigned int modifiers = 0 ) const;


         /**
          * DataMemberAt will return the nth data MemberAt of the At
          * @param  nth data MemberAt
          * @return pointer to data MemberAt
          */
         virtual Member DataMemberAt( size_t nth ) const;


         /**
          * DataMemberByName will return the MemberAt with Name
          * @param  Name of data MemberAt
          * @return data MemberAt
          */
         virtual Member DataMemberByName( const std::string & nam ) const;


         /**
          * DataMemberSize will return the number of data members of this At
          * @return number of data members
          */
         virtual size_t DataMemberSize() const;


         virtual Member_Iterator DataMember_Begin() const;
         virtual Member_Iterator DataMember_End() const;
         virtual Reverse_Member_Iterator DataMember_RBegin() const;
         virtual Reverse_Member_Iterator DataMember_REnd() const;


         /**
          * DeclaringScope will return a pointer to the At of this one
          * @return pointer to declaring At
          */
         virtual Scope DeclaringScope() const;


         /**
          * MemberAt will return the nth MemberAt of the At
          * @param  nth MemberAt
          * @return pointer to nth MemberAt
          */
         virtual Member MemberAt( size_t nth ) const;


         /**
          * MemberByName will return the first MemberAt with a given Name
          * @param  MemberAt Name
          * @return pointer to MemberAt
          */
         virtual Member MemberByName( const std::string & nam,
                                      const Type & signature ) const;

         /**
          * MemberSize will return the number of members
          * @return number of members
          */
         virtual size_t MemberSize() const;


         virtual Member_Iterator Member_Begin() const;
         virtual Member_Iterator Member_End() const;
         virtual Reverse_Member_Iterator Member_RBegin() const;
         virtual Reverse_Member_Iterator Member_REnd() const;


         /**
          * Properties will return a pointer to the PropertyNth list attached
          * to this item
          * @return pointer to PropertyNth list
          */
         virtual PropertyList Properties() const;

      }; // class Union

   } // namespace Reflex
} // namespace ROOT

#include "Reflex/Member.h"

//-------------------------------------------------------------------------------
inline ROOT::Reflex::Member_Iterator ROOT::Reflex::Union::DataMember_Begin() const {
//-------------------------------------------------------------------------------
   return ScopeBase::DataMember_Begin();
}


//-------------------------------------------------------------------------------
inline ROOT::Reflex::Member_Iterator ROOT::Reflex::Union::DataMember_End() const {
//-------------------------------------------------------------------------------
   return ScopeBase::DataMember_End();
}


//-------------------------------------------------------------------------------
inline ROOT::Reflex::Reverse_Member_Iterator ROOT::Reflex::Union::DataMember_RBegin() const {
//-------------------------------------------------------------------------------
   return ScopeBase::DataMember_RBegin();
}


//-------------------------------------------------------------------------------
inline ROOT::Reflex::Reverse_Member_Iterator ROOT::Reflex::Union::DataMember_REnd() const {
//-------------------------------------------------------------------------------
   return ScopeBase::DataMember_REnd();
}


//-------------------------------------------------------------------------------
inline ROOT::Reflex::Member_Iterator ROOT::Reflex::Union::Member_Begin() const {
//-------------------------------------------------------------------------------
   return ScopeBase::Member_Begin();
}


//-------------------------------------------------------------------------------
inline ROOT::Reflex::Member_Iterator ROOT::Reflex::Union::Member_End() const {
//-------------------------------------------------------------------------------
   return ScopeBase::Member_End();
}


//-------------------------------------------------------------------------------
inline ROOT::Reflex::Reverse_Member_Iterator ROOT::Reflex::Union::Member_RBegin() const {
//-------------------------------------------------------------------------------
   return ScopeBase::Member_RBegin();
}


//-------------------------------------------------------------------------------
inline ROOT::Reflex::Reverse_Member_Iterator ROOT::Reflex::Union::Member_REnd() const {
//-------------------------------------------------------------------------------
   return ScopeBase::Member_REnd();  
}


//-------------------------------------------------------------------------------
inline void ROOT::Reflex::Union::AddDataMember( const Member & dm ) const {
//-------------------------------------------------------------------------------
   ScopeBase::AddDataMember( dm );
}


//-------------------------------------------------------------------------------
inline void ROOT::Reflex::Union::AddDataMember( const char * nam,
                                                const Type & typ,
                                                size_t offs,
                                                unsigned int modifiers ) const {
//-------------------------------------------------------------------------------
   ScopeBase::AddDataMember(nam, typ, offs, modifiers );
}


//-------------------------------------------------------------------------------
inline ROOT::Reflex::Member ROOT::Reflex::Union::DataMemberAt( size_t nth ) const {
//-------------------------------------------------------------------------------
   return ScopeBase::DataMemberAt( nth );
}


//-------------------------------------------------------------------------------
inline ROOT::Reflex::Member ROOT::Reflex::Union::DataMemberByName( const std::string & nam ) const {
//-------------------------------------------------------------------------------
   return ScopeBase::DataMemberByName( nam );
}


//-------------------------------------------------------------------------------
inline size_t ROOT::Reflex::Union::DataMemberSize() const {
//-------------------------------------------------------------------------------
   return ScopeBase::DataMemberSize();
}


//-------------------------------------------------------------------------------
inline ROOT::Reflex::Scope ROOT::Reflex::Union::DeclaringScope() const {
//-------------------------------------------------------------------------------
   return ScopeBase::DeclaringScope();
}


//-------------------------------------------------------------------------------
inline ROOT::Reflex::Member ROOT::Reflex::Union::MemberByName( const std::string & nam,
                                                               const Type & signature ) const {
//-------------------------------------------------------------------------------
   return ScopeBase::MemberByName( nam, signature );
}


//-------------------------------------------------------------------------------
inline size_t ROOT::Reflex::Union::MemberSize() const {
//-------------------------------------------------------------------------------
   return ScopeBase::MemberSize();
}


//-------------------------------------------------------------------------------
inline ROOT::Reflex::PropertyList ROOT::Reflex::Union::Properties() const {
//-------------------------------------------------------------------------------
   return ScopeBase::Properties();
}

#endif // ROOT_Reflex_Union

