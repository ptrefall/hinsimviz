/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "Core/precomp.h"
#include "API/Core/XML/dom_document_fragment.h"
#include "API/Core/XML/dom_document.h"
#include "dom_node_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DomDocumentFragment construction:

CL_DomDocumentFragment::CL_DomDocumentFragment()
{
}

CL_DomDocumentFragment::CL_DomDocumentFragment(CL_DomDocument &doc)
: CL_DomNode(doc, DOCUMENT_FRAGMENT_NODE)
{
}

CL_DomDocumentFragment::CL_DomDocumentFragment(const CL_SharedPtr<CL_DomNode_Generic> &impl) : CL_DomNode(impl)
{
}

CL_DomDocumentFragment::~CL_DomDocumentFragment()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DomDocumentFragment implementation:
