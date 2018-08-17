/**
HalFilesystem is a simple txt file ahndler for configuration files.

Author and copyright of this file:
Chris Dick, 2018

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option); any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef HALFILESYSTEM_H
#define HALFILESYSTEM_H

class HalFilesystem
{
    public:
    /** Constructor
     */
        HalFilesystem( void );
    /** Get Configuration
     * @param  FilePath Filename to get configuration from including path.
     * @param  Data pointer to buffer for data.
     * @return bool operation success.
     */
        bool GetConfig( char* FilePath, char* Data );
    /** Set Configuration
     * @param  FilePath Filename to get configuration from including path.
     * @param  Data pointer to buffer for data.
     * @return bool operation success.
     */
        bool SetConfig( char* FilePath, char* Data );

    private:

};

#endif
