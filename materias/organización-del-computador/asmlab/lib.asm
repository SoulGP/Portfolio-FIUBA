extern malloc
extern free
extern getCloneFunction
extern getPrintFunction
extern getDeleteFunction
extern intCmp
extern listAddFirst
extern fprintf
extern listDelete
extern intClone
extern listNew
extern listAddLast
extern listGet
extern listGetSize
extern listPrint
extern intPrint
extern intDelete
extern listClone

global strClone
global strPrint
global strCmp
global strLen
global strDelete

global arrayNew
global arrayDelete
global arrayPrint
global arrayGetSize
global arrayAddLast
global arrayGet
global arrayRemove
global arraySwap

global cardCmp
global cardClone
global cardAddStacked
global cardDelete
global cardGetSuit
global cardGetNumber
global cardGetStacked
global cardPrint
global cardNew

;offsets de array_t
%define ARRAY_TYPE          0
%define ARRAY_SIZE          4
%define ARRAY_CAPACITY      5
%define ARRAY_DATA          8
%define ARRAY_TAMANIO       16

; Offsets para list_t
%define LIST_TYPE           0
%define LIST_SIZE           4
%define LIST_FIRST          8
%define LIST_LAST           16
%define LIST_TAMANIO        24

; Offsets para listElem_t
%define LISTELEM_DATA       0
%define LISTELEM_NEXT       8
%define LISTELEM_PREV       16
%define LISTELEM_TAMANIO    24

; Offsets para card_t
%define CARD_SUIT           0
%define CARD_NUMBER         8
%define CARD_STACKED        16
%define CARD_TAMANIO        24

fmt_char:  db "%c", 0
coma: db ",", 0

fmt_str:    db "%s", 0
null_msg:   db "NULL", 0

section .text

; ** String **
;char* strClone(char* a);
strClone:
    ;Prologo
    push    rbp
    mov     rbp, rsp
    push    rdi                     ;char* a

    call    strLen        

    inc     eax             
    mov     edi, eax                ;tamaño de char* a + 1
    
    sub     rsp, 8        
    call    malloc                  ;reservo memoria para la copia 
    add     rsp, 8  


    pop     rdi            
    mov     rdx, rax       
    mov     rcx, 0         

.while:                             ;copia caracter por caracter
    mov     al, byte [rdi + rcx]
    mov     byte [rdx + rcx], al
    inc     rcx
    cmp     al, 0                   ;si llegamos al final del string salimos del bucle
    jnz     .while

    ;Epilogo
    mov     rax, rdx     

    pop     rbp
ret

;void strPrint(char* a, FILE* pFile)
strPrint:
    push    rbp
    mov     rbp, rsp

    cmp     rdi, 0         ; si a es NULL
    je      .strNulo
    movzx   eax, byte [rdi]
    cmp     al, 0          ; si a apunta a cadena vacía
    je      .strNulo

    ; Llamar a: fprintf(pFile, "%s", a)
    mov     rdx, rdi       ; rdx = a (la cadena)
    mov     rdi, rsi       ; rdi = pFile (archivo de salida)
    lea     rsi, [rel fmt_str]  ; rsi = formato "%s"
    call    fprintf
    jmp     .fin

.strNulo:
    ; Si a es NULL o cadena vacía, imprimir "NULL"
    mov     rdx, null_msg  ; rdx = "NULL"
    mov     rdi, rsi       ; rdi = pFile
    lea     rsi, [rel fmt_str]  ; rsi = formato "%s"
    call    fprintf

.fin:
    pop     rbp
    ret

;uint32_t strLen(char* a);
strLen:
    ;Prologo
    push    rbp
    mov     rbp, rsp
    xor     eax, eax                ;eax = longitud del string

.while:                             ;bucle para contar la longitud
    mov     cl, byte [rdi + rax] 
    cmp     cl, 0                   ;si llegamos al final del string salimos del bucle
    je      .fin
    inc     eax
    jmp     .while

.fin:
    ;Epilogo
    pop     rbp
ret

;int32_t strCmp(char* a, char* b);
strCmp:
    push    rbp
    mov     rbp, rsp
    xor     eax, eax

.while:
    mov     al, byte [rdi]
    mov     bl, byte [rsi]
    cmp     al, bl
    jne     .distintos

    cmp     al, 0
    je      .iguales

    inc     rdi
    inc     rsi
    jmp     .while

.distintos:
    cmp     al, bl
    jb      .primeroMenor
    mov     eax, -1
    jmp     .fin

.primeroMenor:
    mov     eax, 1
    jmp     .fin

.iguales:
    xor     eax, eax
    jmp     .fin

.fin:
    pop     rbp
    ret


;void strDelete(char* a);
strDelete:
    ;Prologo
    push    rbp
    mov     rbp, rsp
    
    call    free        ;liberar memoria del string (rdi)

    ;Epilogo
    pop     rbp
ret


; ** Array **

; uint8_t arrayGetSize(array_t* a)
arrayGetSize:
    push    rbp
    mov     rbp, rsp
    mov     al, [rdi + ARRAY_SIZE]

    pop     rbp

    ret

; void arrayAddLast(array_t* a, void* data)
arrayAddLast:
    ;Prologo
    push    rbp
    mov     rbp, rsp
    push    rbx

    mov     rbx, rdi                            ;rbx = array_t* a

    movzx   rax, byte [rbx + ARRAY_SIZE]        ;rax = size
    movzx   rcx, byte [rbx + ARRAY_CAPACITY]    ;rcx = capacity
    cmp     rax, rcx                            ;si size == capacity no se puede agregar
    je     .fin

    mov     edi, dword [rbx + ARRAY_TYPE]       ;edi = type_t
    sub     rsp, 8  
    call    getCloneFunction                    ;rax = función de clonación para el tipo
    add     rsp, 8

    mov     rdi, rsi                            ;rdi = data a clonar    
    sub     rsp, 8  
    call    rax                                 ;rax = data clonado   
    add     rsp, 8

    mov     rcx, qword [rbx + ARRAY_DATA]         
    movzx   rdx, byte [rbx + ARRAY_SIZE] 
    shl     rdx, 3              
    mov     [rcx + rdx], rax                    ;almacenar el nuevo elemento

    inc     byte [rbx + ARRAY_SIZE]             ;incrementar size

.fin:
    ;Epilogo
    pop     rbx
    pop     rbp
ret

; void* arrayGet(array_t* a, uint8_t i)
arrayGet:
    push    rbp
    mov     rbp, rsp

    movzx   rcx, sil
    movzx   rdx, byte [rdi + ARRAY_SIZE]
    cmp     rcx, rdx
    jae     .out_of_range

    mov     rax, qword [rdi + ARRAY_DATA]
    shl     rcx, 3
    mov     rax, qword [rax + rcx]
    jmp     .fin

.out_of_range:
    xor     rax, rax

.fin:
    pop     rbp
    ret

; array_t* arrayNew(type_t t, uint8_t capacity)
arrayNew:
    ;Prologo
    push    rbp
    mov     rbp, rsp

    push    r12
    push    rdi                                 ;rdi = type_t
    push    rsi                                 ;rsi = capacity

    mov     edi, ARRAY_TAMANIO                  
    sub     rsp, 8
    call    malloc                              ;reservar memoria para un nuevo array_t* nuevo
    add     rsp, 8

    mov     r12, rax                            ;r12 = array_t* nuevo

    pop     rsi
    pop     rdi

    mov     dword [r12 + ARRAY_TYPE], edi       ;nuevo->type = t
    mov     byte [r12 + ARRAY_CAPACITY], sil    ;nuevo->capacity = capacity
    mov     byte [r12 + ARRAY_SIZE], 0          ;nuevo->size = 0

    movzx   edi, byte [r12 + ARRAY_CAPACITY]    ;edi = nuevo->capacity
    shl     rdi, 3
    sub     rsp, 8
    call    malloc                              ;reservar memoria para los datos del array
    add     rsp, 8

    mov     qword [r12 + ARRAY_DATA], rax       ;nuevo->data = memoria reservada de datos

    mov     rdx, rax                            ;rdx = puntero a los datos del array
    movzx   rcx, byte [r12 + ARRAY_CAPACITY]    ;rcx = nuevo->capacity
    xor     rbx, rbx                            ;rbx = índice para inicializar datos

inicializarData:                                ;inicializar los datos del array a 0 (NULL)
    mov     qword [rdx + rbx * 8], 0    
    inc     rbx
    cmp     rbx, rcx                            ;si rbx < capacity salir del bucle
    jl      inicializarData

    ;Epilogo
    mov     rax, r12                            ;devolver puntero al nuevo array_t* inicializado
    pop     r12
    pop     rbp
ret
    
; void* arrayRemove(array_t* a, uint8_t i)
arrayRemove:
    push    rbp
    mov     rbp, rsp
    push    rbx

    mov     rbx, rdi
    movzx   rcx, sil
    movzx   rdx, byte [rbx + ARRAY_SIZE]
    cmp     rcx, rdx
    jae     .out_of_range

    mov     rax, qword [rbx + ARRAY_DATA]   ; rax = data pointer
    shl     rcx, 3                          ; rcx = i * 8
    mov     r8, qword [rax + rcx]           ; r8 = elemento a remover

    ; Mover los elementos a la izquierda para tapar el hueco
    movzx   r9, sil                         ; r9 = i
    inc     r9                              ; r9 = i + 1
    movzx   r10, byte [rbx + ARRAY_SIZE]    ; r10 = size

.corrimiento:
    cmp     r9, r10
    jge     .fin_corrimiento
    mov     r11, qword [rax + r9*8]         ; cargar elemento siguiente
    mov     qword [rax + (r9-1)*8], r11     ; copiarlo a la posición anterior
    inc     r9
    jmp     .corrimiento

.fin_corrimiento:
    dec     byte [rbx + ARRAY_SIZE]         ; size--
    mov     qword [rax + (r10-1)*8], 0     ; limpiar último slot

    mov     rax, r8                         ; devolver puntero removido
    jmp     .fin

.out_of_range:
    xor     rax, rax

.fin:
    pop     rbx
    pop     rbp
    ret


; void arraySwap(array_t* a, uint8_t i, uint8_t j)
arraySwap:
    ; Prologo
    push    rbp
    mov     rbp, rsp
    push    rbx                             ;rbx = array_t* a

    mov     rbx, rdi
    movzx   rax, byte [rbx + ARRAY_SIZE]    ;rax = a->size

    movzx   rcx, sil   
    cmp     rcx, rax
    jae     .fin                            ;si i >= size, salir

    movzx   rcx, dl      
    cmp     rcx, rax
    jae     .fin                            ;si j >= size, salir  

    mov     rax, qword [rbx + ARRAY_DATA]   ;rax = a->data
    
    movzx   rcx, sil    
    movzx   rdx, dl          
    shl     rcx, 3              
    shl     rdx, 3 

    mov     r8, qword [rax + rcx]           ;r8 = a->data[i]
    mov     r9, qword [rax + rdx]           ;r9 = a->data[j]
    mov     qword [rax + rcx], r9           ;a->data[i] = a->data[j]
    mov     qword [rax + rdx], r8           ;a->data[j] = a->data[i]

    .fin:
    ; Epilogo
    pop     rbx
    pop     rbp
ret

; void arrayDelete(array_t* a) 
arrayDelete:
    push    rbp
    mov     rbp, rsp

    push    rbx
    push    r12
    push    r13
    push    r14
    push    r15           
    sub     rsp, 8     

    mov     rbx, rdi                           
    movzx   r15d, byte [rbx + ARRAY_SIZE]      
    mov     r13,  qword [rbx + ARRAY_DATA]            

    mov     eax, dword [rbx + ARRAY_TYPE]
    mov     edi, eax
    call    getDeleteFunction
    mov     r12, rax                           

    xor     r14d, r14d                        
.borrar_elementos:
    cmp     r14d, r15d
    jge     .liberar_data

    mov     rdi, [r13 + r14*8]                 
    test    rdi, rdi
    je      .siguiente

    call    r12                               

.siguiente:
    inc     r14d
    jmp     .borrar_elementos
.liberar_data:

    mov     rdi, r13
    call    free
    mov     rdi, rbx
    call    free

    add     rsp, 8
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     rbp
ret

;void arrayPrint(array_t* a, FILE* pFile)
arrayPrint:
    ;Prologo
    push    rbp
    mov     rbp, rsp
    push    r12           
    push    r13         
    push    r14         
    push    r15         
    push    rbx         

    sub     rsp, 8     

    mov     r13, rdi                        ;r13 = array_t* a
    mov     r12, rsi                        ;r12 = FILE* pFile    

    movzx   edi, byte [r13 + ARRAY_TYPE]    ;edi = a->type_t
    call    getPrintFunction
    mov     r15, rax                        ;r15 = función de impresión para el tipo

    movzx   r14, byte [r13 + ARRAY_SIZE]    ;r14 = a->size

    mov     rdi, r12    
    mov     rsi, fmt_char                   ; rsi = formato "%c"
    mov     edx, '['
    xor     eax, eax                        
    call    fprintf

    xor     ebx, ebx                        ;ebx = índice para recorrer el array  
.while:                                     ;bucle para imprimir los elementos del array
    cmp     rbx, r14
    je     .cerrar_corchete                 ;si i == size, cerrar corchete

    cmp     ebx, 0                          ;si i == 0, no imprimir coma
    jz      .print_elemento
    mov     rdi, r12
    mov     rsi, coma
    xor     eax, eax
    call    fprintf                         ;imprimir coma antes de imprimir elemento

.print_elemento:
    mov     rdx, qword [r13 + ARRAY_DATA]   ;rdx = a->data
    mov     rdi, qword [rdx + rbx*8]        ;rdi = a->data[i]
    mov     rsi, r12
    call    r15                             ;llamar a la función de impresión del tipo

    inc     ebx                             ;incrementar índice
    jmp     .while

.cerrar_corchete:
    mov     rdi, r12
    mov     rsi, fmt_char
    mov     edx, ']'
    xor     eax, eax
    call    fprintf                         ;imprimir corchete de cierre

    ;Epilogo
    add     rsp, 8    
    pop     rbx
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbp
ret

; ** Card **

; card_t* cardNew(char* suit, int32_t* number)
cardNew:
    push    rbp
    mov     rbp, rsp
    push    rbx
    push    r12
    push    r13
    sub     rsp, 8

    mov     rbx, rdi            ; guardar suit en rbx
    mov     r13, rsi            ; guardar number en r13

    mov     edi, CARD_TAMANIO
    call    malloc
    mov     r12, rax            ; r12 = puntero a card_t

    mov     rdi, rbx            ; rdi = suit original
    call    strClone
    mov     qword [r12 + CARD_SUIT], rax   ; guardar suit clonado

    mov     rdi, r13            ; rdi = number
    call    intClone
    mov     qword [r12 + CARD_NUMBER], rax ; guardar puntero a number clonado

    mov     edi, 3              ; TypeCard = 3
    call    listNew
    mov     qword [r12 + CARD_STACKED], rax ; stacked = lista de cartas

    mov     rax, r12            ; devolver puntero a card_t


    add     rsp, 8
    pop     r13
    pop     r12
    pop     rbx
    pop     rbp
    ret

;char* cardGetSuit(card_t* c)
cardGetSuit:
    ;Prologo
    push    rbp
    mov     rbp, rsp

    mov     rax, qword [rdi + CARD_SUIT]        ; rax = card->suit

    ; Epilogo
    pop     rbp
ret

;int32_t* cardGetNumber(card_t* c)
cardGetNumber:
    push    rbp
    mov     rbp, rsp

    mov     rax, qword [rdi + CARD_NUMBER]       ; rax = card->number

    pop     rbp
ret

;list_t* cardGetStacked(card_t* c)
cardGetStacked:
    ;Prologo
    push    rbp
    mov     rbp, rsp

    mov     rax, qword [rdi + CARD_STACKED]      ; rax = card->stacked

    ; Epilogo
    pop     rbp
ret


;void cardPrint(card_t* c, FILE* pFile)
cardPrint:
    push    rbp
    mov     rbp, rsp
    push    rbx
    push    r12


    mov     rbx, rdi        ; rbx = c (card_t*)
    mov     r12, rsi        ; r12 = pFile

    ; Imprimir '{'
    mov     rdi, r12
    lea     rsi, [rel fmt_char]
    mov     edx, '{'
    xor     eax, eax
    call    fprintf

    ; Imprimir suit
    mov     rdi, [rbx + CARD_SUIT]
    mov     rsi, r12
    call    strPrint

    ; Imprimir '-'
    mov     rdi, r12
    lea     rsi, [rel fmt_char]
    mov     edx, '-'
    xor     eax, eax
    call    fprintf

    ; Imprimir number (int32_t*)
    mov     rdi, [rbx + CARD_NUMBER]
    mov     rsi, r12
    call    intPrint

    ; Imprimir '-'
    mov     rdi, r12
    lea     rsi, [rel fmt_char]
    mov     edx, '-'
    xor     eax, eax
    call    fprintf

    ; Imprimir lista de stacked cards
    mov     rdi, [rbx + CARD_STACKED]
    mov     rsi, r12
    call    listPrint

    ; Imprimir '}'
    mov     rdi, r12
    lea     rsi, [rel fmt_char]
    mov     edx, '}'
    xor     eax, eax
    call    fprintf

    pop     r12
    pop     rbx
    pop     rbp
    ret


;int32_t cardCmp(card_t* a, card_t* b)
cardCmp:
    ;Prologo
    push    rbp
    mov     rbp, rsp

    push    r12                                 ;r12 = card_t* a
    push    r13                                 ;r13 = card_t* b

    mov     r12, rdi 
    mov     r13, rsi    

    mov     rdi, qword [r12 + CARD_SUIT]        ; rdi = a->suit
    mov     rsi, qword [r13 + CARD_SUIT]        ; rsi = b->suit
    call    strCmp                              ; comparar los suits de las cartas
    cmp     eax, 0                              ; si a->suit == b->suit
    jne     .fin

    mov     rdi, qword [r12 + CARD_NUMBER]     ; rdi = a->number
    mov     rsi, qword [r13 + CARD_NUMBER]     ; rsi = b->number
    call    intCmp                             ; comparar los números de las cartas  

.fin:   
    ; Epilogo
    pop     r13
    pop     r12
    pop     rbp
ret

;card_t* cardClone(card_t* c)
cardClone:
    cardClone:
    push    rbp
    mov     rbp, rsp
    push    rbx
    push    r14

    mov     rbx, rdi            ; card_t* c

    mov     rdi, qword [rbx + CARD_SUIT]
    mov     rsi, qword [rbx + CARD_NUMBER]
    call    cardNew             ; rax = new card
    mov     r14, rax

    mov     rdi, qword [r14 + CARD_STACKED]
    call    listDelete          ; (lista está vacía)

    mov     rdi, qword [rbx + CARD_STACKED]
    call    listClone           ; rax = lista clonada
    mov     qword [r14 + CARD_STACKED], rax

    mov     rax, r14            ; return

    pop     r14
    pop     rbx
    pop     rbp
    ret

;void cardAddStacked(card_t* c, card_t* card)
cardAddStacked:
    ; Prologo
    push    rbp
    mov     rbp, rsp
    push    rbx                                     ; rbx = card_t* c
    sub     rsp, 8

    mov     rbx, rdi             

    mov     rdi, qword [rbx + CARD_STACKED]         ; rdi = c->stacked (lista de cartas)    
    call    listAddFirst                            ; agregar la carta al inicio de la lista

    ; Epilogo
    add     rsp, 8
    pop     rbx
    pop     rbp
ret


;void cardDelete(card_t* c)
cardDelete:
    push    rbp
    mov     rbp, rsp
    push    rbx

    mov     rbx, rdi                ; rbx = c

    ; Borrar suit
    mov     rdi, [rbx + CARD_SUIT]
    sub     rsp, 8
    call    strDelete
    add     rsp, 8

    ; Borrar number
    mov     rdi, [rbx + CARD_NUMBER]
    call    intDelete

    mov     rdi, [rbx + CARD_STACKED]
    call    listDelete

    ; Liberar la estructura card_t
    mov     rdi, rbx
    call    free

    pop     rbx
    pop     rbp
    ret