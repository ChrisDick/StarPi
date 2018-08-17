/*
HalFilesystem is a simple txt file handler for configuration files

Author and copyright of this file:
Chris Dick, 2018

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

#include "HalFilesystem.h"

/** Constructor 
 */
HalFilesystem::HalFilesystem()
{

}

/** Get Configuration 
 */
bool HalFilesystem::GetConfig( char* FilePath, char* Data )
{
    bool Result = false;
    ifstream ConfigFile (FilePath);
    if( ConfigFile.is_open())
    {
        string DataString;
        getline ( ConfigFile, DataString );
        const char *Temp = DataString.c_str();
        strcpy( Data, Temp );
        Result = true;
        ConfigFile.close();
    }
    return( Result );
}

/** Set Configuration
 */
bool HalFilesystem::SetConfig( char* FilePath, char* Data )
{
    bool Result = false;
    ofstream ConfigFile ( FilePath, ios::trunc );
    if ( ConfigFile.is_open() )
    {
        std::string DataString( Data );
        ConfigFile << DataString;
        Result = true;
        ConfigFile.close();
    }
    return( Result );
}
