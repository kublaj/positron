/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://wwwt.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1999 Netscape Communications Corporation.  All Rights
 * Reserved.
 */

#include "nsHTMLEditFactory.h"
#include "nsIHTMLEditor.h"
#include "nsHTMLEditor.h"
#include "nsEditor.h"
#include "nsEditorCID.h"
#include "nsRepository.h"

static NS_DEFINE_IID(kISupportsIID,        NS_ISUPPORTS_IID);
static NS_DEFINE_IID(kIFactoryIID,         NS_IFACTORY_IID);
static NS_DEFINE_IID(kIHTMLEditorIID,      NS_IHTMLEDITOR_IID);
static NS_DEFINE_IID(kHTMLEditorCID,       NS_HTMLEDITOR_CID);
static NS_DEFINE_IID(kIHTMLEditFactoryIID, NS_IHTMLEDITORFACTORY_IID);


nsresult
GetHTMLEditFactory(nsIFactory **aFactory, const nsCID & aClass)
{
  static nsCOMPtr<nsIFactory>  g_pNSIFactory;
  PR_EnterMonitor(getEditorMonitor());
  nsresult result = NS_ERROR_FAILURE;
  if (!g_pNSIFactory)
  {
    nsHTMLEditFactory *factory = new nsHTMLEditFactory(aClass);
    g_pNSIFactory = do_QueryInterface(factory);
    if (factory)
      result = NS_OK;
  }
  result = g_pNSIFactory->QueryInterface(kIFactoryIID, (void **)aFactory);
  PR_ExitMonitor(getEditorMonitor());
  return result;
}

////////////////////////////////////////////////////////////////////////////
// from nsISupports 

NS_METHOD
nsHTMLEditFactory::QueryInterface(const nsIID& aIID, void** aInstancePtr) 
{
  if (nsnull == aInstancePtr) {
    NS_NOTREACHED("!nsEditor");
    return NS_ERROR_NULL_POINTER; 
  } 
  if (aIID.Equals(kIFactoryIID) ||
    aIID.Equals(kISupportsIID)) {
    *aInstancePtr = (void*) this; 
    AddRef();  
    return NS_OK; 
  }
  return NS_NOINTERFACE; 
}

NS_IMPL_ADDREF(nsHTMLEditFactory)
NS_IMPL_RELEASE(nsHTMLEditFactory)


////////////////////////////////////////////////////////////////////////////
// from nsIFactory:

NS_METHOD
nsHTMLEditFactory::CreateInstance(nsISupports *aOuter, REFNSIID aIID, void **aResult)
{
  *aResult  = nsnull;
  nsISupports *obj = nsnull;
  if (!aResult)
    return NS_ERROR_NULL_POINTER;
  if (aOuter && !aIID.Equals(kISupportsIID))
    return NS_NOINTERFACE;   // XXX right error?


  if (mCID.Equals(kHTMLEditorCID))
    obj = (nsISupports *)new nsHTMLEditor();
  //more class ids to support. here

  if (obj && NS_FAILED(obj->QueryInterface(aIID, (void**)aResult)) ) 
  {
    delete obj;
    return NS_NOINTERFACE;
  }
  return NS_OK;
}



NS_METHOD
nsHTMLEditFactory::LockFactory(PRBool aLock)
{
  return NS_OK;
}



////////////////////////////////////////////////////////////////////////////

nsHTMLEditFactory::nsHTMLEditFactory(const nsCID &aClass)
:mCID(aClass)
{
  NS_INIT_REFCNT();
}

nsHTMLEditFactory::~nsHTMLEditFactory()
{
  //nsRepository::UnregisterFactory(mCID, (nsIFactory *)this); //we are out of ref counts anyway
}







