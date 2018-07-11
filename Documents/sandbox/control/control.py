from Tkinter import *
import time
import threading
import sys
import socket
import serial
from functools import partial


class W1:
	
	def __init__(self, mode=0, ip="192.168.4.1", port=30001):
		# mode = 0: only print,  1: serial   2: udp
		
		self.mode = mode
		
		self.root = Tk()
		self.root.geometry("1000x500")
		
		self.root.title("Quadruped Controller")
		
		if(mode == 1):
			# LINUX:
			#self.ser = serial.Serial('/dev/ttyACM1', 115200)
			# WIN:
			self.ser = serial.Serial('COM8', 115200)
		elif (mode == 2):
			self.ip = ip
			self.port = port
			self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

			
		# -------------------------------------------------------------------------	
		
		self.buttons = {}
		self.labels = {}
		self.scales = {}
		self.spinboxs = {}
		
		# -------------------------------------------------------------------------	
		
		self.autoMode = False

		self.speedMax = 10
		self.speedMin = 1
		
		self.stepsMax = 20
		self.stepsMin = 1
		
		self.speed = IntVar()
		self.speed.set(5)
		
		self.steps = IntVar()
		self.steps.set(1)

		self.dir = 0
		self.rotation = 0
		
		self.camV = IntVar()
		self.camV.set(90)
		self.camH = IntVar()
		self.camH.set(90)
		
		# -------------------------------------------------------------------------	
		
		self.initElements()
		
		# -------------------------------------------------------------------------	
		
		self.activeKeys = set()
		self.root.bind_all('<KeyPress>', self.keyPressHandler)
		self.root.bind_all('<KeyRelease>', self.keyReleaseHandler)
		
		# -------------------------------------------------------------------------	
		
		self.speed.trace("w", self.update_sp)
		self.steps.trace("w", self.update_st)
		self.camV.trace("w", self.update_cam)
		self.camH.trace("w", self.update_cam)

		self.root.after(2000, self.handleKeyActions)
		self.root.focus_set()
		self.root.mainloop()



	def initElements(self):

		movframe = LabelFrame(self.root, text="Movement")
		movframe.place(x=10,y=0)
		c = Label(movframe, text=" ")
		c.pack(padx=300,pady=110)
		
		camframe = LabelFrame(self.root, text="Camera control")
		camframe.place(x=600,y=0)
		c = Label(camframe, text=" ")
		c.pack(padx=190,pady=110)
	
		# ------ Scales ------

		# Speed
		sc_sp = Scale(self.root, variable=self.speed, from_=self.speedMin, to=self.speedMax, repeatdelay=1, showvalue=0, orient=HORIZONTAL)
		sc_sp.place(x=70, y=190)
		self.scales['sc_sp'] = sc_sp
		
		# Steps
		sc_st = Scale(self.root, variable=self.steps, from_=self.stepsMin, to=self.stepsMax, repeatdelay=1, showvalue=0, orient=HORIZONTAL)
		sc_st.place(x=70, y=220)
		self.scales['sc_st'] = sc_st
		
		
		# Cam V
		sc_cv = Scale(self.root, variable=self.camV, from_=0, to=180, repeatdelay=1, showvalue=0, orient=VERTICAL)
		sc_cv.place(x=680, y=70)
		self.scales['sc_cv'] = sc_cv
		
		# Cam H
		sc_ch = Scale(self.root, variable=self.camH, from_=0, to=180, repeatdelay=1, showvalue=0, orient=HORIZONTAL)
		sc_ch.place(x=800, y=190)
		self.scales['sc_ch'] = sc_ch
		

		# ------ Spin Box ------

		# Speed
		sp_sp = Spinbox(self.root, from_=self.speedMin, to=self.speedMax, repeatdelay=1, width=10, textvariable=self.speed)
		sp_sp.place(x=190, y=190)
		self.spinboxs['sp_sp'] = sp_sp
		
		# Steps
		sp_st = Spinbox(self.root, from_=self.stepsMin, to=self.stepsMax, repeatdelay=1, width=10, textvariable=self.steps)
		sp_st.place(x=190, y=220)
		self.spinboxs['sp_st'] = sp_st
		
		# Cam V
		sp_cv = Spinbox(self.root, from_=0, to=180, repeatdelay=1, width=10, textvariable=self.camV)
		sp_cv.place(x=650, y=190)
		self.spinboxs['sp_cv'] = sp_cv
		
		# Cam h
		sp_ch = Spinbox(self.root, from_=0, to=180, repeatdelay=1, width=10, textvariable=self.camH)
		sp_ch.place(x=910, y=190)
		self.spinboxs['sp_ch'] = sp_ch
		

		# ------ Labels ------
		
		# Movement
		ff = Label(self.root, text="forward (w)")
		ff.place(x=193,y=20)
		
		bw = Label(self.root, text="backward (s)")
		bw.place(x=193,y=135)
		
		r = Label(self.root, text="right (d)")
		r.place(x=325,y=102)
		
		l = Label(self.root, text="left (a)")
		l.place(x=82,y=102)

		rr = Label(self.root, text="rotate right (e)")
		rr.place(x=325,y=30)
		
		rl = Label(self.root, text="rotate right (q)")
		rl.place(x=46,y=30)
		
		sp = Label(self.root, text="Speed:", fg="black")
		sp.place(x=20,y=190)
		
		s = Label(self.root, text="Steps:", fg="black")
		s.place(x=20,y=220)
		
		# Camera
		tilt = Label(self.root, text="Tilt", fg="black")
		tilt.place(x=700,y=30)
		
		pan = Label(self.root, text="Pan", fg="black")
		pan.place(x=850,y=30)
		
		up = Label(self.root, text="    Up Key ^", fg="grey47")
		up.place(x=610,y=70)
		
		dw = Label(self.root, text="Down Key v", fg="grey47")
		dw.place(x=610,y=155)
		
		lb_x = Label(self.root, text="< Right Key", fg="grey47")
		lb_x.place(x=750,y=220)
		
		lb_y = Label(self.root, text="Left Key >", fg="grey47")
		lb_y.place(x=889,y=220)
		
		
		# ------ Buttons ------

		self.buttons['fw'] = Button(self.root, text="^", width=5, command=partial(self.setDir, (1)))
		self.buttons['fw'].place(x=200, y=50)
		
		self.buttons['bw'] = Button(self.root, text="v", width=5, command=partial(self.setDir, (3)))
		self.buttons['bw'].place(x=200, y=100)
		
		self.buttons['r'] = Button(self.root, text=">", width=5, command=partial(self.setDir, (2)))
		self.buttons['r'].place(x=270, y=100)
		
		self.buttons['l'] = Button(self.root, text="<", width=5, command=partial(self.setDir, (4)))
		self.buttons['l'].place(x=130, y=100)
		
		self.buttons['rr'] = Button(self.root, text="/->", width=5, command=partial(self.setRot, (1)))
		self.buttons['rr'].place(x=270, y=50)
		
		self.buttons['rl'] = Button(self.root, text="<-\\", width=5, command=partial(self.setRot, (-1)))
		self.buttons['rl'].place(x=130, y=50)
		
		self.buttons['wu'] = Button(self.root, text="wake up ^", width=10)
		self.buttons['wu'].place(x=500, y=30)
		
		self.buttons['si'] = Button(self.root, text="sleep   v", width=10)
		self.buttons['si'].place(x=500, y=70)
		
		self.buttons['st'] = Button(self.root, text="STOP", borderwidth=4, width=10)
		self.buttons['st'].place(x=500, y=110)
		
		self.buttons['auto'] = Button(self.root, text="Auto", borderwidth=4, width=10, command=self.setAuto, relief="raised")
		self.buttons['auto'].place(x=420, y=200)
		
		self.buttons['man'] = Button(self.root, text="Manual", borderwidth=4, width=10, command=self.setMan, relief="sunken", state=DISABLED)
		self.buttons['man'].place(x=500, y=200)
		
		
		# ------ Canvas ------
		self.canv = Canvas(self.root, height=120, width=200)
		self.canv.place(x=710, y=60)
		self.arcV = self.canv.create_arc((10,10,110,110), start=90,extent=180, fill="snow")
		self.arcH = self.canv.create_arc((100,50,200,150), start=0,extent=180, fill="snow")
		
		self.RectV = self.canv.create_arc((10,10,110,110), start=90,extent=self.camV.get(), fill="snow", outline="blue")
		self.arcH = self.canv.create_arc((100,50,200,150), start=0, extent=180-self.camH.get(), fill="snow", outline="red")

		
		
		
		
	def setAuto(self):
		self.autoMode = True
		
		self.buttons["auto"].config(relief="sunken")
		self.buttons["auto"].config(state=DISABLED)
		
		self.buttons["man"].config(relief="raised")
		self.buttons["man"].config(state=ACTIVE)
		
		self.buttons["fw"].config(state=DISABLED)
		self.buttons["bw"].config(state=DISABLED)
		self.buttons["r"].config(state=DISABLED)
		self.buttons["l"].config(state=DISABLED)
		self.buttons["rr"].config(state=DISABLED)
		self.buttons["rl"].config(state=DISABLED)
		self.buttons["wu"].config(state=DISABLED)
		self.buttons["si"].config(state=DISABLED)
		self.buttons["st"].config(state=DISABLED)
		
		self.send()
		
		
	def setMan(self):
		self.autoMode = False
		
		self.buttons["man"].config(relief="sunken")
		self.buttons["man"].config(state=DISABLED)
		
		self.buttons["auto"].config(relief="raised")
		self.buttons["auto"].config(state=ACTIVE)
		
		self.buttons["fw"].config(state=ACTIVE)
		self.buttons["bw"].config(state=ACTIVE)
		self.buttons["r"].config(state=ACTIVE)
		self.buttons["l"].config(state=ACTIVE)
		self.buttons["rr"].config(state=ACTIVE)
		self.buttons["rl"].config(state=ACTIVE)
		self.buttons["wu"].config(state=ACTIVE)
		self.buttons["si"].config(state=ACTIVE)
		self.buttons["st"].config(state=ACTIVE)
		
		self.send()
		
		
	def update_cam(self, *args):
		self.canv.delete("all")
		self.arcV = self.canv.create_arc((10,10,110,110), start=90,extent=180, fill="snow")
		self.arcH = self.canv.create_arc((100,50,200,150), start=0,extent=180, fill="snow")

		self.RectV = self.canv.create_arc((10,10,110,110), start=90,extent=self.camV.get(), fill="snow", outline="blue")
		self.arcH = self.canv.create_arc((100,50,200,150), start=0, extent=180-self.camH.get(), fill="snow", outline="red")
		
		self.send()
	
		
	
	def update_sp(self, *args):
		print self.speed.get()
		self.send()
	
	def update_st(self, *args):
		print self.steps.get()
		self.send()

	def setDir(self, *args):
		self.dir = args[0]
		print self.dir
		self.send()
		
	def setRot(self, *args):
		self.rot = args[0]
		print self.rot
		self.send()
	
	
	
	def send(self):
		self.dir = 0
		self.rot = 0
		print 'hey :D'
		
		
		
	
	# ============================ Keys events ============================
	
	def keyPressHandler(self, event):
		self.activeKeys.add(event.keycode)
		

	def keyReleaseHandler(self, event):
		self.activeKeys.remove(event.keycode)
		
		
	def handleKeyActions(self):
		print self.activeKeys
		
		for k in self.activeKeys:
			if k == 38:		# Up key
				if (self.camV.get() > 0):
					self.camV.set(self.camV.get() - 5)
				pass
					
			if k == 40:		# Down key
				if (self.camV.get() < 180):
					self.camV.set(self.camV.get() + 5)
				pass
				
			if k == 39:		# Right key
				if (self.camH.get() < 180):
					self.camH.set(self.camH.get() + 5)
				pass
				
			if k == 37:		# Left key
				if (self.camH.get() > 0):
					self.camH.set(self.camH.get() - 5)
				pass
				
			if k == 87:		# W key
				self.setDir(1)
				pass
			
			if k == 68:		# D key
				self.setDir(2)
				pass
				
			if k == 83:		# S key
				self.setDir(3)
				pass
			
			if k == 65:		# A key
				self.setDir(4)
				pass
			
			if k == 69:		# E key
				self.setRot(1)
				pass
			
			if k == 81:		# Q key
				self.setRot(-1)
				pass
		
		
		self.root.after(80, self.handleKeyActions)
	
	# ====================================================================
			
	
	
	def send_pos(self, *args):
		
		if(args[0] == None):
		
			possI = [self.leg_RFx.get(),self.leg_RFy.get(),self.leg_RFz.get(),
					self.leg_LFx.get(),self.leg_LFy.get(),self.leg_LFz.get(),
					self.leg_RBx.get(),self.leg_RBy.get(),self.leg_RBz.get(),
					self.leg_LBx.get(),self.leg_LBy.get(),self.leg_LBz.get()]
		
			for i in range(12):
				if not isinstance(possI[i], int):
					possI = None
					break
		else:
			possI = args[0]
		
		if possI != None:
			
			possS = []
			msg = ""
			for i in range(12):
				ii = possI[i]
				ss = ""
				
				if (ii>=0):
					ss=ss+"+"
				elif (ii<0):
					ss=ss+"-"
				
				if(abs(ii) < 10):
					ss=ss+"0"
				
				ss=ss+str(abs(ii))
				possS.append(ss)
				msg = msg+ss
			
			
			print msg
			if (self.mode == 1):
				self.ser.write("ss" + msg + ";")
			elif (self.mode == 2):
				self.sock.sendto('ss'+msg+';', (self.ip, self.port))

	
	
	

if (__name__ == "__main__"):
	if len(sys.argv) == 1:
		print '"control.py [serial / udp]"'
		w1 = W1()
	else:

		if (sys.argv[1] == "serial"):
			w1 = W1(1)
		elif (sys.argv[1] == "udp"):
			w1 = W1(2)
