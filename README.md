# Qubed

**Qubed**  (with a *very dignified* capital **Q**, because it's fancy like that ✨) is a custom-built Vulkan-based rendering engine, developed from scratch with love, caffeine, and cat-like curiosity. It's a personal playground and learning project, inspired very loosely by TheCherno’s *Cubed*—but with original architecture and implementation. This engine is entirely independent and not based on any external rendering framework.

This is a **Work-in-Progress (WIP)**, so things might break, crash, or spontaneously combust (hopefully not literally). Expect rapid changes, wild experiments, and possibly a sprinkle of magic dust ✨

---

## 🗂️ Structure

```bash
src/
├── device.cpp / device.hpp     → Physical & logical device setup  
├── instance.cpp / instance.hpp → Vulkan instance & validation layers  
├── utils.cpp / utils.hpp       → Utility / Helper Functions  
├── window.cpp / window.hpp     → GLFW window creation & callbacks  
├── main.cpp                    → Main setup logic

