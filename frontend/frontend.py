import tkinter as tk
from tkinter import ttk, messagebox
import subprocess
import os

# ---------------- PATHS ----------------
BACKEND_DIR = os.path.abspath("../backend")
BACKEND_PATH = os.path.join(BACKEND_DIR, "todo.exe")
TASKS_FILE = os.path.join(BACKEND_DIR, "tasks.txt")

# ---------------- DATA ----------------
def load_tasks():
    tasks = []
    if not os.path.exists(TASKS_FILE):
        return tasks

    with open(TASKS_FILE, "r") as f:
        for line in f:
            id, title, created, deadline, mark = line.strip().split("|")
            status = "✔" if mark == "1" else ""
            tasks.append((id, title, created, deadline, status))
    return tasks


def refresh_tasks():
    tree.delete(*tree.get_children())
    for task in load_tasks():
        tree.insert("", "end", values=task)


def refresh_with_delay():
    root.after(500, refresh_tasks)

# ---------------- BACKEND CALLS ----------------
def add_task():
    title = title_entry.get().strip()
    deadline = deadline_entry.get().strip()

    if not title or not deadline:
        messagebox.showerror("Error", "Title and deadline required")
        return

    subprocess.run(
        [BACKEND_PATH, "add", title, deadline],
        cwd=BACKEND_DIR,
        creationflags=subprocess.CREATE_NO_WINDOW
    )

    title_entry.delete(0, tk.END)
    deadline_entry.delete(0, tk.END)

    refresh_tasks()   # ← no delay needed now



def delete_task():
    selected = tree.focus()
    if not selected:
        return

    task_id = tree.item(selected)["values"][0]

    subprocess.run(
        [BACKEND_PATH, "delete", str(task_id)],
        cwd=BACKEND_DIR,
        creationflags=subprocess.CREATE_NO_WINDOW
    )

    refresh_tasks()


def toggle_complete():
    selected = tree.focus()
    if not selected:
        messagebox.showwarning("Select task", "Please select a task first")
        return

    task_id = tree.item(selected)["values"][0]

    subprocess.run(
        [BACKEND_PATH, "complete", str(task_id)],
        cwd=BACKEND_DIR,
        creationflags=subprocess.CREATE_NO_WINDOW
    )

    refresh_tasks()


def sort_tasks():
    subprocess.Popen(
        [BACKEND_PATH, "sort"],
        cwd=BACKEND_DIR,
        creationflags=subprocess.CREATE_NO_WINDOW
    )

    refresh_tasks()

# ---------------- UI ----------------
root = tk.Tk()
root.title("To-Do List")
root.geometry("750x450")
root.configure(bg="#f4f4f4")

style = ttk.Style()
style.theme_use("clam")
style.configure("Treeview", rowheight=28, font=("Segoe UI", 10))
style.configure("Treeview.Heading", font=("Segoe UI", 10, "bold"))

# ---------------- TABLE ----------------
columns = ("ID", "Title", "Created", "Deadline", "Done")
tree = ttk.Treeview(root, columns=columns, show="headings")

for col in columns:
    tree.heading(col, text=col)
    tree.column(col, anchor="center")

tree.column("Title", width=250)
tree.pack(pady=20, padx=20, fill="both", expand=True)

# ---------------- INPUT ----------------
input_frame = tk.Frame(root, bg="#f4f4f4")
input_frame.pack(pady=10)

title_entry = tk.Entry(input_frame, width=30)
title_entry.grid(row=0, column=0, padx=5)
title_entry.insert(0, "Task title")

deadline_entry = tk.Entry(input_frame, width=15)
deadline_entry.grid(row=0, column=1, padx=5)
deadline_entry.insert(0, "YYYYMMDD")

# ---------------- BUTTONS ----------------
btn_frame = tk.Frame(root, bg="#f4f4f4")
btn_frame.pack(pady=10)

tk.Button(btn_frame, text="➕ Add", width=12, command=add_task).grid(row=0, column=0, padx=5)
tk.Button(btn_frame, text="🗑 Delete", width=12, command=delete_task).grid(row=0, column=1, padx=5)
tk.Button(btn_frame, text="✔ Complete",width=12, command=toggle_complete).grid(row=0, column=2, padx=5)
tk.Button(btn_frame, text="⏱ Sort by Deadline", width=18, command=sort_tasks).grid(row=0, column=3, padx=5)

# ---------------- START ----------------
refresh_tasks()
root.mainloop()
