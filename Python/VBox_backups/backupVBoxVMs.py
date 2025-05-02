import os
import shutil
from datetime import datetime

VM_BASE_PATH = r"C:\Users\mday39\VirtualBox VMs\Backup_VMs"
BACKUP_ROOT_PATH = r"C:\Users\mday39\Box\Box\VM-backups"
LOG_FILE = os.path.join(BACKUP_ROOT_PATH, "backup_log.txt")

def get_date_stamp():
    """Return a timestamp formatted as YYYY-MM-DD."""
    return datetime.now().strftime("%Y-%m-%d")

def get_latest_backup():
    """Find the latest valid backup folder based on date-based timestamp."""
    backups = []
    for folder in os.listdir(BACKUP_ROOT_PATH):
        if folder.startswith("VM-backup_"):
            try:
                timestamp_str = folder.split("_")[-1]
                datetime.strptime(timestamp_str, "%Y-%m-%d")  # Validate format
                backups.append(folder)
            except ValueError:
                continue  # Skip incorrectly named folders
    
    return os.path.join(BACKUP_ROOT_PATH, max(backups)) if backups else None

def buffered_copy(source, dest, buffer_size=64 * 1024):
    """Copy file in chunks to avoid memory overload."""
    try:
        with open(source, 'rb') as f_in, open(dest, 'wb') as f_out:
            while chunk := f_in.read(buffer_size):
                f_out.write(chunk)
        print(f"Backup completed: {dest}")
    except Exception as e:
        print(f"Copy failed: {e}")

def log_backup(vm_name, date_stamp):
    """Record backup timestamps in a log file."""
    with open(LOG_FILE, "a") as log:
        log.write(f"{vm_name} backed up on {date_stamp}\n")

if __name__ == "__main__":
    latest_backup_path = get_latest_backup()
    date_stamp = get_date_stamp()
    new_backup_path = os.path.join(BACKUP_ROOT_PATH, f"VM-backup_{date_stamp}")

    changes_detected = False

    if os.path.exists(VM_BASE_PATH):
        for vm_folder in os.listdir(VM_BASE_PATH):
            vm_path = os.path.join(VM_BASE_PATH, vm_folder)
            if os.path.isdir(vm_path):
                backup_vm_folder = os.path.join(new_backup_path, vm_folder)
                os.makedirs(backup_vm_folder, exist_ok=True)

                for file in os.listdir(vm_path):
                    file_path = os.path.join(vm_path, file)
                    backup_file_path = os.path.join(backup_vm_folder, file)

                    if os.path.isfile(file_path):
                        # Ensure backup only happens if the file was modified
                        if latest_backup_path:
                            prev_backup_file = os.path.join(latest_backup_path, vm_folder, file)
                            if os.path.exists(prev_backup_file) and os.path.getmtime(file_path) <= os.path.getmtime(prev_backup_file):
                                continue  # Skip unchanged file

                        buffered_copy(file_path, backup_file_path)
                        changes_detected = True  # Mark that at least one file changed

                if changes_detected:
                    log_backup(vm_folder, date_stamp)

        # Remove unused backup folder to avoid clutter
        if not changes_detected:
            shutil.rmtree(new_backup_path)

    else:
        print(f"Error: VM directory not found at {VM_BASE_PATH}")
