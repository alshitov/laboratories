import os
import shutil
import subprocess
import stat
from typing import Optional
from stat import \
    S_IRUSR, S_IWUSR, S_IXUSR, \
    S_IRGRP, S_IWGRP, S_IXGRP, \
    S_IROTH, S_IWOTH, S_IXOTH


class DirUtil:
    def list_directory(self, directory) -> (list, list, list):
        files = []
        directories = []
        list_items = []

        for entry in os.listdir(directory):
            full_path = os.path.join(directory, entry)

            is_dir = self.dir_exists(full_path)
            if is_dir:
                directories.append(entry)

            if self.file_exists(full_path):
                files.append(entry)

            entry_str = '{0}\t{1}\t{2}'
            size = round(os.path.getsize(full_path) / 1024, 3)
            access_rights = self.get_access_rights(full_path)

            list_items.append(
                entry_str.format(
                    entry + '/' if is_dir else entry,
                    '%s KB' % format(size, '.3f'),
                    access_rights
                )
            )

        list_items.extend(['./', '../'])

        return (files, directories, list_items)

    def get_access_rights(self, record: str) -> str:
        mode = os.stat(record).st_mode
        is_dir = os.path.isdir(record)

        return ('d' if is_dir else '') + ''.join([
            'r' if bool(mode & S_IRUSR) else '-',
            'w' if bool(mode & S_IWUSR) else '-',
            'x' if bool(mode & S_IXUSR) else '-',
            'r' if bool(mode & S_IRGRP) else '-',
            'w' if bool(mode & S_IWGRP) else '-',
            'x' if bool(mode & S_IXGRP) else '-',
            'r' if bool(mode & S_IROTH) else '-',
            'w' if bool(mode & S_IWOTH) else '-',
            'x' if bool(mode & S_IXOTH) else '-'
        ])

    def add_file(self, record: str) -> Optional[int]:
        try:
            open(record, 'a').close()
        except FileExistsError:
            return 1
        except PermissionError:
            return 2

    def add_directory(self, record: str) -> Optional[int]:
        try:
            os.mkdir(record)
        except FileExistsError:
            return 1
        except PermissionError:
            return 2

    def remove_file(self, record: str) -> Optional[int]:
        try:
            os.remove(record)
        except FileNotFoundError:
            return 1
        except PermissionError:
            return 2

    def rename_file(self, record: str, new_record: str) -> Optional[int]:
        try:
            os.rename(record, new_record)
        except FileNotFoundError:
            return 1
        except PermissionError:
            return 2

    def move_file(self, record: str, new_record: str) -> Optional[int]:
        return self.rename_file(record, new_record)

    def remove_directory(self, record: str) -> Optional[int]:
        try:
            os.rmdir(record)
        except FileNotFoundError:
            return 1
        except PermissionError:
            return 2
        except OSError:
            return 3

    def rename_directory(self, record: str, new_record: str) -> Optional[int]:
        try:
            os.rename(record, new_record)
        except FileNotFoundError:
            return 1
        except FileNotFoundError:
            return 1
        except FileExistsError:
            return 4

    def move_directory(self, record: str, new_record: str) -> Optional[int]:
        return self.rename_directory(record, new_record)

    def clear(self, record: str) -> str:
        return os.path.abspath(record)

    def full_path(self, directory: str, record: str) -> str:
        return self.clear(os.path.join(directory, record))

    def rel_path(self, minuend: str, minuend_base: str, subtrahend_base: str):
        return os.path.join(
            self.clear(subtrahend_base),
            self.clear(minuend).replace(minuend_base, '')
        )

    def dir_exists(self, entry):
        return os.path.isdir(entry)

    def file_exists(self, entry):
        return os.path.isfile(entry)
