CIRCUITO = combinacionales.circ
TOPLEVEL = verificador
SALIDA_PROPIA = out.txt
SALIDA_CATEDRA = salida_catedra.txt
LOGISIM = logisim-evolution

.PHONY: all run check clean

all: check

run:
	@echo "Ejecutando Logisim-Evolution..."
	@$(LOGISIM) --toplevel-circuit "$(TOPLEVEL)" -tty table $(CIRCUITO) > $(SALIDA_PROPIA)

check: run
	@echo "Comparando salidas..."
	@diff -q $(SALIDA_PROPIA) $(SALIDA_CATEDRA) > /dev/null && \
	echo "✅ ¡Los resultados coinciden con la salida de la cátedra!" || \
	(echo "❌ Los resultados NO coinciden con la salida de la cátedra." && exit 1)

clean:
	@rm $(SALIDA_PROPIA)
