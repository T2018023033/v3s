SUBDIRS += net-server/     
SUBDIRS += thread-pool/
SUBDIRS += uart-mesh/
SUBDIRS += cJSON/
SUBDIRS += msg-queue/
SUBDIRS += sqlitedbms-api/
SUBDIRS += mem-share/
SUBDIRS += cmd-deal/

TOP_DIR = $(PWD)
OBJ = mainApp
SRCS := $(foreach subdir, $(SUBDIRS), $(wildcard $(TOP_DIR)/$(subdir)/*.c))
SRCS += $(wildcard *.c)
CUR_OBJS := $(patsubst %.c, %.o, $(wildcard *.c))
OBJS := $(patsubst %.c, %.o, $(SRCS))

CFLAGS += $(foreach subdir, $(SUBDIRS), "-I$(TOP_DIR)/$(subdir)")
CFLAGS += -I$(TOP_DIR)
LDFLAGS += -lpthread -lm -ldl -Wall -lrt --std=c99

export CFLAGS

$(OBJ): subdirs $(CUR_OBJS)
	@$(CC) -o $@ $(CFLAGS) $(OBJS) $(LDFLAGS)

%.o:%.c
	@$(CC) -c $(CFLAGS) $< -o $@

subdirs: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: clean subdirs $(SUBDIRS)
clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
		done
	@rm -f *.o $(OBJ)

