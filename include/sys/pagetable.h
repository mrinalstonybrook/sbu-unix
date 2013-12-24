struct PML4e {
    long value;
};

struct PDPE {
    long value;
};

struct PDE {
    long value;
};

struct PTE {
    long value;
};

struct PML4 {
    struct PML4e entries[512];
};

struct PDP {
    struct PDPE entries[512];
};

struct PD {
    struct PDE entries[512];
};

struct PT {
    struct PTE entries[512];
};
