#!/usr/bin/env python

import usb	# importa o nosso modulo
import sys
from time import sleep

# Acha um dispositivo no sistema.
# Se nao for encontrado, retorna None
def find_device(busses, idProduct, idVendor):
	for bus in busses:
		for dev in bus.devices:
			if dev.idProduct == idProduct and dev.idVendor == idVendor:
				return dev

# Escreve msg no endpoint bulk e depois
# le, se o que foi lido for igual a msg,
# teste ok, senao, imprime mensagem de
# erro e encerra script
def test_bulk(handle, msg):
	handle.bulkWrite(0x2, msg, 1000)
	data = handle.bulkRead(0x82, 1000, 1000)
	str = "".join([chr(i) for i in data]) # transforma em string
	if str != msg:
		print "Error in bulk test!!!"
		sys.exit(1)

# mesmo que test_bulk mas para transferencias interrupt
def test_interrupt(handle, msg):
	handle.interruptWrite(0x1, msg, 1000)
	data = handle.interruptRead(0x81, 1000, 1000)
	str = "".join([chr(i) for i in data])
	if str != msg:
		print "Error in interrupt test!!!"
		sys.exit(1)


if __name__ == "__main__":		# modulo princial?
	print "********************************"
	print "PyUSB test script"
	print "********************************"
	print ""

	busses = usb.busses()	# varre os barramentos

	# teste de enumeracao. Tenta encontrar o nosso hardware
	# idProduct = 0xc e idVendor = 0x555
	print "enumeration device test..."
	dev = find_device(busses, 0x000c, 0x0555)
	if dev is None:		# se nao encontrou, imprime erro e sai
		print "enumeration test failed..."
		sys.exit(1)
	
	print "enumeration test ok..."	# enumeracao ok
	
	# abre o dispositivo
	print "opening device test..."
	handle = dev.open()
	print "opeing device ok..."

	# define a configuracao a usar
	print "configuration test..."
	handle.setConfiguration(1)
	print "configuration test ok..."

	# se apropria da interface
	print "claim interface test..."
	handle.claimInterface(0)
	print "claim interface test ok..."

	# configura o alterna setting da interface
	print "interface configuration test..."
	handle.setAltInterface(0)
	print "interface configuration ok..."

	# testa I/O escreve e le dados dos endpoints interrupt
	# e bulk diversas vezes
	print "I/O test..."

	for x in range(40):
		test_interrupt(handle, "interrupt test 1")
		test_bulk(handle, "bulk test 1")
		test_interrupt(handle, "interrupt test 2")
		test_bulk(handle, "bulk test 2")

	print "I/O test ok..."

	print "reset endpoint test..."
	# Essa funcao esta com problemas no Windows.
	# Sempre quando eh chamada levanta uma excessao dizendo
	# que o parametro eh invalido. O erro vem do driver da libusb
	# Problema em estudo
	handle.resetEndpoint(2)	
	print "reset endpoint ok..."

	# Testa para ver se esta lendo as strings...
	print "string test..."
	print "string index 1:", handle.getString(1,100)
	print "string index 2:", handle.getString(2,100)
	print "string test ok..."

	# Le o descritor do dispositivo para ver se funcao
	# getDescriptor esta funcionando
	print "descriptor test..."
	descr = handle.getDescriptor(1, 0, 18)
	if len(descr) != 18:	# tamanho do device descriptor eh 18
		print "descriptor test failed..."
		sys.exit(1)
	else:
		print "descriptor:", descr
	print "descriptor test ok..."

	# efetua uma requisicao GET_STATUS para verificar se transferencia
	# do tipo control estah ok
	print "control transfer test..."
	control_res = handle.controlMsg(0x80, 0, 2, 0, 0, 1000)
	if len(control_res) != 2:	# retornam dois bytes de status
		print "control transfer test failed..."
		sys.exit(1)
	else:
		print "control tranfer result: ", control_res
	print "control transfer ok..."

	# Teste do reset:
	# 1) a funcao reset reseta o dispositivo
	# 2) o barramento eh varrido novamente
	# 3) se achar, teste de reset ok, senao, erro
	print "reset device test..."
	handle.reset()

	del handle
	del dev
	del busses

	# necessario para funcionar em Windows. Se comentar
	# a linha abaixo, ocorre um GPF
	sleep(3)

	busses = usb.busses()

	dev = find_device(busses, 0x000c, 0x0555)
	if dev is None:
		print "reset device test failed..."
	else:
		print "reset device test ok..."

	del dev
	del busses
	del usb
	
	print "type enter key to exit"
	raw_input()
