
import tkinter as tk
from tkinter import ttk, messagebox

def load_transactions():
    try:
        with open("transactions.txt", "r") as file:
            lines = file.readlines()
            return [line.strip().split(",") for line in lines]
    except FileNotFoundError:
        messagebox.showerror("Error", "transactions.txt not found!")
        return []

def search_student():
    student_id = entry_id.get().strip()
    if not student_id:
        messagebox.showwarning("Input Error", "Please enter a Student ID.")
        return

    transactions = load_transactions()
    filtered = [t for t in transactions if t[0] == student_id]

    for row in tree.get_children():
        tree.delete(row)

    if filtered:
        for trans in filtered:
            tree.insert("", tk.END, values=trans)
    else:
        messagebox.showinfo("No Records", f"No transactions found for Student ID: {student_id}")

root = tk.Tk()
root.title("Student Transactions Viewer")
root.geometry("700x400")

tk.Label(root, text="Enter Student ID:").pack(pady=5)
entry_id = tk.Entry(root)
entry_id.pack(pady=5)

btn_search = tk.Button(root, text="Search", command=search_student)
btn_search.pack(pady=5)

cols = ("Student ID", "Student Name", "Book ID", "Book Title", "Status", "Date")
tree = ttk.Treeview(root, columns=cols, show="headings")
for col in cols:
    tree.heading(col, text=col)
    tree.column(col, width=100)
tree.pack(expand=True, fill="both", pady=10)

root.mainloop()

