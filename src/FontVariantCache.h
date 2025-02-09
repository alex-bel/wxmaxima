// -*- mode: c++; c-file-style: "linux"; c-basic-offset: 2; indent-tabs-mode: nil -*-
//
//  Copyright (C) 2023 Gunter Königsmann <wxMaxima@peterpall.de>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//
//  SPDX-License-Identifier: GPL-2.0+

#ifndef FONTVARIANTCACHE_H
#define FONTVARIANTCACHE_H

#include "precomp.h"
#include <memory>
#include <wx/font.h>
#include <functional>
#include <list>
#include <utility>
#include <unordered_map>

/*! \file
 * This file implements the wxFont cache system.

 This system is necessary since creating a wxFont object costs loads of
 CPU cycles costs.

 Each font gets its own FontVariantCache that caches all sizes of each
 style of that font that we ever generated a wxFont object for.
 */
class FontVariantCache final
{
  FontVariantCache(const FontVariantCache &) = delete;
  FontVariantCache &operator=(const FontVariantCache &) = delete;
public:
  //! Creates a font variant cache for the font named fontName.
  explicit FontVariantCache(wxString fontName);
  ~FontVariantCache(){}
  //! Clear this font variant cache
  void ClearCache();
  /*! Returns a font with the requested attributes
    
    This font can be either cached or newly created.*/
  std::shared_ptr<wxFont> GetFont (double size,
                                   bool isItalic,
                                   bool isBold,
                                   bool isUnderlined,
                                   bool isSlanted,
                                   bool isStrikeThrough
    );
  //! Get the name of the fonts this font variant cache is responsible for
  const wxString& GetFaceName() const {return m_fontName;}
private:
  //! Get the number of the internal cache hashmap
  static int GetIndex (
    bool isItalic,
    bool isBold,
    bool isUnderlined,
    bool isSlanted,
    bool isStrikeThrough
    )
    {
      int result =0;
      if(isItalic)
        result++;
      if(isBold)
        result += 2;
      if(isUnderlined)
        result +=4;
      if(isSlanted)
        result +=8;
      if(isStrikeThrough)
        result +=16;
      return result;
    }

  //! One hashmap for each value GetIndex() can return.
  mutable std::unordered_map<double, std::shared_ptr<wxFont>> m_fontCaches[32];
  //! The name our font cache
  wxString m_fontName;
};

#endif  // FONTVARIANTCACHE_H
