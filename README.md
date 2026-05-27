# Mark, intérprete de Markdown de Cesitah

Mark *pretende ser* un intérprete de Markdown, aunque actualmente es solo un programa con un par de métodos y pruebas unitarias no muy exhaustivas.

Tengo varias ideas para el intérprete:

- Implementar todo (o al menos la mayoría) de la [especificación de Markdown 1.0.1](https://daringfireball.net/projects/markdown/).
- Implementar al menos parte de la [extensión de Markdown realizada por Rentry](https://rentry.co/how).
- Implementar extensión para poder poner *bookmarks*, IDs en cualquier parte del documento que luego se puedan incluir en un link.
- Ofrecer, al menos, conversión a HTML.
- Interpretar directamente carpetas enteras, de forma recursiva, si se reciben.
- Interpretar links a distintas secciones (y *bookmarks*) en el mismo documento y en documentos del mismo árbol de carpetas.

## Ejemplos

### Links a otros archivos de la misma carpeta

La idea es que si tu tienes la siguiente carpeta:

```
proyecto
├── README.md
├── articulos
│   ├── ARTICULO-1.md
│   └── ARTICULO-2.md
└── portales
    ├── img
    │   ├── 01.png
    │   └── 02.png
    ├── MUSICA.md
    └── SERIES.md
```

Y quieres enlazar al artículo 1 desde el archivo *README.md* lo puedas hacer usando la ruta relativa.

```markdown
[Ir a la cabecera](articulos/ARTICULO-1).
```

### Links a secciones

Aquí se seguiría la [extensión de Markdown realizada por Rentry](https://rentry.co/how). Los títulos se convierten a Strings sin caracteres especiales y con las letras en minúscula. Por ejemplo, *Capítulo 8: El principio del fin* se traduciría a la ID `#capitulo-8-el-principio-del-fin`.

Las secciones que no sean traducibles de esta forma recibirán una ID única sacada, por ejemplo, de un contador. El intérprete lanzará un aviso en estos casos.

Por supuesto, los links a secciones funcionarían incluso si la ruta lleva a otro archivo.

### Links a *bookmarks*

Los *bookmarks* (o marcadores) serían una extensión propia de Markdown. La idea es poder asignar una ID a un punto concreto del archivo para hacer links de forma fácil y segura, sin hacks raros.

Una sintaxis posible sería:

```markdown
Este documento contiene un bookmark en el siguiente párrafo.

#[marcador] De esa forma, es muy fácil hacer un link que vaya hasta este punto del documento, sin poner ningún título.

Para ir al párrafo anterior sigue [este link](#marcador).
```

Si la ID es inválida (por ejemplo, está repetida), el intérprete deberá dar un aviso y asignar una ID única como en el caso de las secciones.

## Compilar y ejecutar

Para compilarlo, se asume que dispones de [GNU Make](https://www.gnu.org/software/make/). Dentro de la *Makefile* hay una sección llamada *Dependencias* que detalla los programas que son necesarios para compilar el programa.

También hay una sección llamada *Dependencias opcionales*. El programa se puede compilar sin esos programas, pero ofrecen funcionalidades que te podrían interesar.

Se puede compilar el programa ejecutando `make` o `make all` en la carpeta raíz del repositorio. Esto generará un ejecutable en la carpeta *target*. Para compilarlo y ejecutarlo con un solo comando, ejecuta `make run`.

La *Makefile* incluye información sobre las distintas recetas y opciones de compilación que se pueden usar con el programa.

## Licencia

Todos los archivos de este repositorio se encuentran bajo la [GNU General Public License 3.0](https://www.gnu.org/licenses/gpl-3.0.en.html). Eso significa que puedes compartir y adaptar mi trabajo siempre y cuando lo distribuyas como [software libre](https://www.gnu.org/philosophy/floss-and-foss.html).

Puedes encontrar el texto completo de la licencia en el archivo `COPYING`.