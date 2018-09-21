/*
 * Copyright 2018 Longri
 *
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
 *
 * Created by Longri on 22.07.18.
 */

#ifndef CACHEBOX_FILEFILTER_H
#define CACHEBOX_FILEFILTER_H

class FileFilter {
public:
    /**
     * Tests whether or not the specified abstract pathname should be
     * included in a pathname list.
     *
     * @param  pathname  The abstract pathname to be tested
     * @return  <code>true</code> if and only if <code>pathname</code>
     *          should be included
     */
    virtual bool accept(char *filePath) const = 0;

};

#endif //CACHEBOX_FILEFILTER_H

#ifndef CACHEBOX_EMPTYFILEFILTER_H
#define CACHEBOX_EMPTYFILEFILTER_H

class EmptyFileFilter : public FileFilter {
public:
    /**
     * Tests whether or not the specified abstract pathname should be
     * included in a pathname list.
     *
     * @param  pathname  The abstract pathname to be tested
     * @return  <code>true</code> if and only if <code>pathname</code>
     *          should be included
     */
    virtual bool accept(char *filePath) const override {
        return true;
    }

};

#endif //CACHEBOX_EMPTYFILEFILTER_H

#ifndef CACHEBOX_FILEHANDLE_H
#define CACHEBOX_FILEHANDLE_H


#include "String16.h"
#include "System.h"
#include "exceptions/IllegalStateException.h"
#include "FileReader.h"
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>


#if defined _WIN32

#include <windows.h>

#else

#include <dirent.h>

#endif


#define EMPTY_STRING String16("")
#define PARENT_PATH String16("../")
#define RELATIVE_PATH String16("./")

/** Represents a file or directory on the filesystem!
 *
 * this class is inspired by FileHandle from LibGdx
 *
 */
class FileHandle {
private:
    String16 absolutePath;
    long namePos = -1;
    long parentPos = -1;

    void searchPos(long &pos, long from) {
        if (pos >= 0)return;
        pos = absolutePath.lastIndexOf('/');
        if (pos >= 0)pos++;
        else {
            // maybe replace '\\'
            absolutePath.replaceAll(String16("\\"), String16("/"));
            pos = absolutePath.lastIndexOf('/');
            if (pos >= 0)pos++;
        }
    }

    static void emptyDirectory(FileHandle *file) {
        if (file->isDirectory()) {
            std::vector<FileHandle> files = file->list();
            for (int i = 0, n = files.size(); i < n; i++) {
                if (!files[i].isDirectory())
                    files[i]._delete();
                else
                    deleteDirectory(&files[i]);
            }
        }
    }

    static bool deleteDirectory(FileHandle *file) {
        emptyDirectory(file);

#if defined _WIN32
        return _rmdir(file->getPath()) != -1;
#else
        return file->_delete();
#endif
    }

    static bool removeParentIndicator(String16 &dir) {
        if (dir.startsWith(PARENT_PATH)) {
            dir.replace(PARENT_PATH, EMPTY_STRING);
            return true;
        }
        return false;
    }

    static void removeInnerParentIndicator(String16 &pathString) {
        if (pathString.contains(PARENT_PATH)) {
            long pos = pathString.indexOf(PARENT_PATH);
            long slashPos = pathString.lastIndexOf(U'/', pos);
            slashPos = pathString.lastIndexOf(U'/', slashPos);
            String16 sub = pathString.subString(0, slashPos + 1);
            String16 st = pathString.subString(pos + 3);
            sub.append(st);
            pathString.set(sub);
            removeInnerParentIndicator(pathString);
        }
    }

    static void resolveAbsolutePath(String16 &pathString) {
        if (pathString.startsWith(PARENT_PATH)) {

            String16 actWorkingDir = getWorkingPath();
            long cnt = 0;
            while (removeParentIndicator(pathString)) {
                cnt++;
            }

            long pos = actWorkingDir.length();
            for (int i = 0; i < cnt; ++i) {
                pos = actWorkingDir.lastIndexOf('/', pos);
            }

            String16 sub = actWorkingDir.subString(0, pos);
            sub.append('/');
            sub.append(pathString);
            pathString.set(sub);
        } else if (pathString.startsWith(RELATIVE_PATH)) {
            String16 actWorkingDir = getWorkingPath();
            pathString.replaceAll(RELATIVE_PATH, EMPTY_STRING);
            actWorkingDir.append('/');
            actWorkingDir.append(pathString);
            pathString.set(actWorkingDir);
        }
        removeInnerParentIndicator(pathString);
    }

    /**
    * 1= File
    * 2= Dir
    * 0= not exist
    * -1= error
    * @return
    */
    int getStat() {
        struct stat s;
        const char *tmpCStringPath = getUtf8Path();
        int ret = 0;
        if (stat(getUtf8Path(), &s) == 0) {
            if (s.st_mode & S_IFDIR) {
                //it's a directory
                ret = 2;
            } else if (s.st_mode & S_IFREG) {
                //it's a file
                ret = 1;
            } else {
                //something else
                ret = 0;
            }
        } else {
            //error
            ret = -1;
        }
        delete[] tmpCStringPath;
        return ret;
    }


    std::vector<FileHandle> list(const FileFilter &filter, bool withFilter) {
        std::vector<FileHandle> vec = std::vector<FileHandle>();
        String16 listPathString(absolutePath);

#if defined _WIN32
        listPathString.append("/*");
#endif
        char *pathCString = String16::toUtf8_Chars(listPathString);


#if defined _WIN32
        WIN32_FIND_DATA ffd;
        LARGE_INTEGER filesize;
        size_t length_of_arg;
        HANDLE hFind = INVALID_HANDLE_VALUE;

        // Find the first file in the directory.
        hFind = FindFirstFile(pathCString, &ffd);

        if (INVALID_HANDLE_VALUE == hFind)
            return vec; // return empty vector

        do {
            if (!strcmp(ffd.cFileName, ".")) continue;
            if (!strcmp(ffd.cFileName, "..")) continue;
            FileHandle item = this->child(ffd.cFileName);
            if (withFilter) {
                char *cStringPath = item.getPath();
                if (filter.accept(cStringPath))
                    vec.emplace_back(item);
                delete[]cStringPath;
            } else {
                vec.emplace_back(item);
            }
        } while (FindNextFile(hFind, &ffd) != 0);
        FindClose(hFind);
#else
        DIR *dirFile = opendir(pathCString);
        if (dirFile) {
            struct dirent *hFile;
            errno = 0;
            while ((hFile = readdir(dirFile)) != nullptr) {
                if (!strcmp(hFile->d_name, ".")) continue;
                if (!strcmp(hFile->d_name, "..")) continue;
                FileHandle item = this->child(hFile->d_name);
                if (withFilter) {
                    char *cStringPath = item.getPath();
                    if (filter.accept(cStringPath))
                        vec.emplace_back(item);
                    delete[]cStringPath;
                } else {
                    vec.emplace_back(item);
                }
            }
            closedir(dirFile);
        }
#endif
        delete[] pathCString;
        return vec;
    }


public:


    static String16 getWorkingPath() {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        String16 str = String16(cwd);
        str.replaceAll(String16("\\"), String16("/"));
        return str;
    }

    static FileHandle getWorkingHandle() {
        return FileHandle(getWorkingPath());
    }

    explicit FileHandle(String16 path) : absolutePath(path) {
        resolveAbsolutePath(absolutePath);
    }

    explicit FileHandle(char *path) : absolutePath(String16(path)) {
        resolveAbsolutePath(absolutePath);
    }

    String16 getAbsolutePath() {
        return absolutePath;
    }


    /** @return the name of the file, without any parent paths. */
    String16 name() {
        searchPos(namePos, absolutePath.length());
        if (namePos < 0)return String16(absolutePath);
        return absolutePath.subString(namePos);
    }

    String16 extension() {
        long dotIndex = absolutePath.lastIndexOf('.');
        if (dotIndex == -1) return String16("");
        return absolutePath.subString(dotIndex + 1);
    }

    /** @return the name of the file, without parent paths or the extension. */
    String16 pathWithoutExtension() {
        long dotIndex = absolutePath.lastIndexOf('.');
        if (dotIndex == -1) return String16(absolutePath);
        return absolutePath.subString(0, dotIndex);
    }


    /** Returns a handle to the child with the specified name. */
    FileHandle child(String16 name) {
        String16 newPath(absolutePath);
        newPath.append('/');
        newPath.append(name);
        return FileHandle(newPath);
    }

    /** Returns a handle to the child with the specified name. */
    FileHandle child(const char *name) {
        String16 newPath(absolutePath);
        newPath.append('/');
        newPath.append(name);
        return FileHandle(newPath);
    }

    /** Returns a handle to the parent. */
    FileHandle parent() {
        searchPos(namePos, absolutePath.length());
        searchPos(parentPos, namePos);
        if (parentPos == -1) {
            String16 msg = String16("Can't return Parent directory from: ");
            msg.append(this->absolutePath);
            throw NumberFormatException(String16::toUtf8_Chars(msg), __FILE__, __FUNCTION__, __LINE__);
        }
        FileHandle fh(absolutePath.subString(0, parentPos - 1));
        return fh;
    }


    /**
     * You must destroy this Char Array
     * @return 
     */
    const char *getUtf8Path() {
        return String16::toUtf8_Chars(absolutePath);
    }

    char *getPath() {
        return String16::toUtf8_Chars(absolutePath);
    }

    /** Returns true if the file exists.*/
    bool exists() {
        return getStat() == 1;
    }

    /** Returns true if this file is a directory.*/
    bool isDirectory() {
        return getStat() == 2;
    }

    /** Returns the paths to the children of this directory. Returns an empty list if this file handle represents a file and not a
	 * directory. On the desktop, an {@link FileType#Internal} handle to a directory on the classpath will return a zero length
	 * array.
     */
    std::vector<FileHandle> list() {
        return list(EmptyFileFilter(), false);
    }

    /** Returns the paths to the children of this directory that satisfy the specified filter. Returns an empty list if this file
	 * handle represents a file and not a directory. On the desktop, an {@link FileType#Internal} handle to a directory on the
	 * classpath will return a zero length array.
	 * @param filter the {@link FileFilter} to filter files
	 * @throws GdxRuntimeException if this file is an {@link FileType#Classpath} file. */
    std::vector<FileHandle> list(const FileFilter &filter) {
        return list(filter, true);
    }

    /** Returns the File size as byte or -1 if not exist or directory
     *
     * @return
     */
    long fileSize() {
        std::streampos begin, end;

        const char *a = getUtf8Path();
        std::ifstream myfile(a, std::ios::binary);
        begin = myfile.tellg();
        myfile.seekg(0, std::ios::end);
        end = myfile.tellg();
        myfile.close();
        delete[] a;
        return (long) (end - begin);
    }

    /** Writes the specified String16 to the file using the utf8 charset. Parent directories will be created if necessary.
	 * @param append If false, this file will be overwritten if it exists, otherwise it will be appended.
	 *
     */
    void writeString(String16 string, bool append) {
        FILE *pFile;
        char *buffer = String16::toUtf8_Chars(string);//TODO write with bufferdStream
        const char *path = getUtf8Path();
        if (append)
            pFile = fopen(path, "ab+");
        else
            pFile = fopen(path, "wb");
        size_t byteLength = std::strlen(buffer);
        fwrite(buffer, sizeof(char), byteLength, pFile);
        fclose(pFile);

        delete[] path;
        delete[] buffer;
    }

    String16 readString() {
        const char *pathCharArray = getUtf8Path();
        FileReader fileReader(pathCharArray);
        String16 retString = fileReader.readString();
        delete[] pathCharArray;
        return retString;
    }


    void mkdirs() {
        mkdirParent(this);
    }

    void mkdirParent(FileHandle *handle) {
        FileHandle parent = FileHandle(String16());
        try {
            parent = handle->parent();
        } catch (NumberFormatException &exception) {
            // cant create parent dir
            return;
        }
        if (parent.isDirectory()) {
            const char *pathCharArray = handle->getUtf8Path();
#if defined(_WIN32)
            _mkdir(pathCharArray);
#else
            mode_t nMode = 0733; // UNIX style permissions
            mkdir(pathCharArray, nMode);
#endif
            delete[] pathCharArray;
        } else {
            mkdirParent(&parent);
        }
    }

    /** Deletes this file or empty directory and returns success. Will not delete a directory that has children.*/
    bool _delete() {
        const char *pathCharArray = getUtf8Path();
        bool ret = remove(pathCharArray) == 0;
        delete[] pathCharArray;
        return ret;
    }

    /** Deletes this file or directory and all children, recursively.*/
    bool deleteDirectory() {
        return deleteDirectory(this);
    }


    void copy(FileHandle target) {

        //if target exist, delete
        if (target.exists()) {
            target._delete();
        }

        //make dirs, if needed 
        FileHandle parent = target.parent();
        if (!parent.isDirectory()) {
            parent.mkdirs();
        }

        char buf[BUFSIZ];
        size_t size;

        const char *sourcePath = this->getUtf8Path();
        const char *targetPath = target.getUtf8Path();

        FILE *source = fopen(sourcePath, "rb");
        FILE *dest = fopen(targetPath, "wb");

        // clean and more secure
        // feof(FILE* stream) returns non-zero if the end of file indicator for stream is set

        size = fread(buf, 1, BUFSIZ, source);
        while (size) {
            fwrite(buf, 1, size, dest);
            size = fread(buf, 1, BUFSIZ, source);
        }

        fclose(source);
        fclose(dest);

        delete[] sourcePath;
        delete[] targetPath;

    }

};


#endif //CACHEBOX_FILEHANDLE_H

