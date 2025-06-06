# c-stack-benchmarks
Realizando uma pequena comparacao entre estruturas de pilhas em C, utilizando (1) lista encadeada, (2) array e (3) g_queue da biblioteca GLib

## Instalando dependencias (Debian based)
```bash
  sudo apt install build-essential libglib2.0-dev -y
```

## Compilando
```bash
  gcc -O3 -march=native -flto -o main main.c  `pkg-config --cflags --libs glib-2.0`
```

## Executando
```bash
  ./main
```
