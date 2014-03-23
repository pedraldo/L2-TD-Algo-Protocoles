CC  = gcc
SYS = -Wall -pedantic -DUNIX
INC = -I.

SRCDIR = src
BINDIR = bin
OBJDIR = obj
FILESDIR = fichiers

OBJ_CONF   = config.o
OBJ_PHY    = services_physique.o
OBJ_LINK   = couche_liaison.o
OBJ_APP_NC = appli_non_connectee.o
OBJ_APP_C  = appli_connectee.o

OBJ_COMMON = $(OBJ_CONF) $(OBJ_PHY) $(OBJ_LINK)

OBJ_LINK_1_E = proto_liaison_v1_emetteur.o
OBJ_LINK_1_R = proto_liaison_v1_recepteur.o

OBJ_LINK_2_E = proto_liaison_v2_emetteur.o
OBJ_LINK_2_R = proto_liaison_v2_recepteur.o

OBJ_LINK_3_E = proto_liaison_v3_emetteur.o
OBJ_LINK_3_R = proto_liaison_v3_recepteur.o

OBJ_LINK_4_E = proto_liaison_v4_emetteur.o
OBJ_LINK_4_R = proto_liaison_v4_recepteur.o

OBJ_LINK_5_E = proto_liaison_v5_emetteur.o
OBJ_LINK_5_R = proto_liaison_v5_recepteur.o


# Mode NC, pas de controle de flux, ni reprise sur erreurs
# ---------------------------------------------------------
sl1: $(OBJ_COMMON) $(OBJ_APP_NC) $(OBJ_LINK_1_E) $(OBJ_LINK_1_R)
	$(CC) -o $(BINDIR)/emetteur $(OBJ_COMMON) $(OBJ_APP_NC) $(OBJ_LINK_1_E)
	$(CC) -o $(BINDIR)/recepteur $(OBJ_COMMON) $(OBJ_APP_NC) $(OBJ_LINK_1_R)
	mv *.o $(OBJDIR)/

# Mode NC, controle de flux S&W, pas de reprise sur erreurs
# ----------------------------------------------------------
sl2: $(OBJ_COMMON) $(OBJ_APP_NC) $(OBJ_LINK_2_E) $(OBJ_LINK_2_R)
	$(CC) -o $(BINDIR)/emetteur $(OBJ_COMMON) $(OBJ_APP_NC) $(OBJ_LINK_2_E)
	$(CC) -o $(BINDIR)/recepteur $(OBJ_COMMON) $(OBJ_APP_NC) $(OBJ_LINK_2_R)
	mv *.o $(OBJDIR)/

# Mode NC, controle de flux et reprise sur erreurs S&W PAR
# ---------------------------------------------------------
sl3: $(OBJ_COMMON) $(OBJ_APP_NC) $(OBJ_LINK_3_E) $(OBJ_LINK_3_R)
	$(CC) -o $(BINDIR)/emetteur $(OBJ_COMMON) $(OBJ_APP_NC) $(OBJ_LINK_3_E)
	$(CC) -o $(BINDIR)/recepteur $(OBJ_COMMON) $(OBJ_APP_NC) $(OBJ_LINK_3_R)
	mv *.o $(OBJDIR)/

# Mode C, controle de flux et reprise sur erreurs S&W PAR
# --------------------------------------------------------
sl4: $(OBJ_COMMON) $(OBJ_APP_C) $(OBJ_LINK_4_E) $(OBJ_LINK_4_R)
	$(CC) -o $(BINDIR)/emetteur $(OBJ_COMMON) $(OBJ_APP_C) $(OBJ_LINK_4_E)
	$(CC) -o $(BINDIR)/recepteur $(OBJ_COMMON) $(OBJ_APP_C) $(OBJ_LINK_4_R)
	mv *.o $(OBJDIR)/

# Mode C, fenetre d'anticipation & go back n
# --------------------------------------------
sl5: $(OBJ_COMMON) $(OBJ_APP_C) $(OBJ_LINK_5_E) $(OBJ_LINK_5_R)
	$(CC) -o $(BINDIR)/emetteur $(OBJ_COMMON) $(OBJ_APP_C) $(OBJ_LINK_5_E)
	$(CC) -o $(BINDIR)/recepteur $(OBJ_COMMON) $(OBJ_APP_C) $(OBJ_LINK_5_R)
	mv *.o $(OBJDIR)/


# '%' matches filename
# $@  for the pattern-matched target
# $<  for the pattern-matched dependency	
%.o: $(SRCDIR)/%.c
	$(CC) -o $@ -c $< $(SYS) $(INC)

clean:
	rm -f *.o
	rm -f $(OBJDIR)/*.o
	rm -f $(BINDIR)/* $(FILESDIR)/out.*
	

