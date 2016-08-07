/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * http://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include "fileutils.h"
#include "filepath.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {

/*****************************************************************************************
 *  Static Methods
 ****************************************************************************************/

void FileUtils::copyFile(const FilePath& source, const FilePath& dest) throw (Exception)
{
    if (!source.isExistingFile()) {
        throw LogicError(__FILE__, __LINE__, QString(),
            QString(tr("The file \"%1\" does not exist."))
            .arg(source.toNative()));
    }
    if (dest.isExistingFile() || dest.isExistingDir()) {
        throw LogicError(__FILE__, __LINE__, QString(),
            QString(tr("The file or directory \"%1\" exists already."))
            .arg(dest.toNative()));
    }
    if (!QFile::copy(source.toStr(), dest.toStr())) {
        throw RuntimeError(__FILE__, __LINE__, QString(),
            QString(tr("Could not copy file \"%1\" to \"%2\"."))
            .arg(source.toNative(), dest.toNative()));
    }
}

void FileUtils::copyDirRecursively(const FilePath& source, const FilePath& dest) throw (Exception)
{
    if (!source.isExistingDir()) {
        throw LogicError(__FILE__, __LINE__, QString(),
            QString(tr("The directory \"%1\" does not exist."))
            .arg(source.toNative()));
    }
    if (dest.isExistingFile() || dest.isExistingDir()) {
        throw LogicError(__FILE__, __LINE__, QString(),
            QString(tr("The file or directory \"%1\" exists already."))
            .arg(dest.toNative()));
    }
    if (!dest.mkPath()) {
        throw LogicError(__FILE__, __LINE__, QString(),
            QString(tr("Could not create directory \"%1\"."))
            .arg(dest.toNative()));
    }
    QDir sourceDir(source.toStr());
    foreach (const QString& file, sourceDir.entryList(QDir::Files | QDir::Hidden)) {
        copyFile(source.getPathTo(file), dest.getPathTo(file));
    }
    foreach (const QString& dir, sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
        copyDirRecursively(source.getPathTo(dir), dest.getPathTo(dir));
    }
}

void FileUtils::removeFile(const FilePath& file) throw (Exception)
{
    if (!QFile::remove(file.toStr())) {
        throw RuntimeError(__FILE__, __LINE__, QString(),
            QString(tr("Could not remove file \"%1\".")).arg(file.toNative()));
    }
}

void FileUtils::removeDirRecursively(const FilePath& dir) throw (Exception)
{
    if (!QDir(dir.toStr()).removeRecursively()) {
        throw RuntimeError(__FILE__, __LINE__, QString(),
            QString(tr("Could not remove directory \"%1\"."))
            .arg(dir.toNative()));
    }
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb