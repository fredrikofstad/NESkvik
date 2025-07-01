#include "cpu.h"

#include "address_modes.h"
#include "lookup.h"
#include "bus.h"

CPU::CPU() {
}

uint8_t CPU::read(uint16_t address) const {
    return bus->read(address);
}

void CPU::write(uint16_t address, uint8_t data) {
    return bus->write(address, data);
}

uint8_t CPU::getFlag(flags::flags flag) {
    return ((status & flag) > 0) ? 1 : 0;
}

void CPU::setFlag(flags::flags flag, bool value) {
    if (value) {
        status |= flag;
    } else {
        status &= ~flag;
    }
}

void CPU::clock() {
    if (cycles == 0) {
        current_opcode = read(pc);

        if (lookup[current_opcode].opcode == nullptr || lookup[current_opcode].addressmode == nullptr) {
            printf("ERROR: lookup table entry for opcode %02X is null!\n", current_opcode);
            exit(1);
        }
        pc++;  // advance PC after fetching

        setFlag(flags::Unused, true);

        cycles = lookup[current_opcode].cycles;

        uint8_t additional_cycle1 = lookup[current_opcode].addressmode(*this);
        uint8_t additional_cycle2 = lookup[current_opcode].opcode(*this);

        cycles += (additional_cycle1 & additional_cycle2);

        if (trace) {
            printf("TRACE PC=%04X OPCODE=%02X Fetched=%02X A=%02X X=%02X Y=%02X SP=%02X STATUS=%02X\n",
                   pc - 1, current_opcode, fetched_data, a, x, y, sp, status);
        }
    }
    cycles--;
}

void CPU::reset() {
    uint16_t low  = bus->read(0xFFFC);
    uint16_t high = bus->read(0xFFFD);
    pc = (high << 8) | low;

    a = 0;
    x = 0;
    y = 0;
    sp = 0xFD;
    status = 0x00 | flags::Unused;

    address_abs = 0x0000;
    address_rel = 0x0000;
    fetched_data = 0x00;
    cycles = 8;
}

void CPU::irq() {

    if (getFlag(flags::Interrupts) != 0) return;

    write(0x0100 + sp, (pc >> 8) & 0x00FF);
    sp--;
    write(0x0100 + sp, pc & 0x00FF);
    sp--;

    setFlag(flags::Break, false);
    setFlag(flags::Unused, true);
    setFlag(flags::Interrupts, true);
    write(0x0100 + sp, status);
    sp--;

    address_abs = 0xFFFE;
    uint16_t low = read(address_abs + 0);
    uint16_t high = read(address_abs + 1);
    pc = (high << 8) | low;

    cycles = 7;
}

// unignorable interrupt
void CPU::nmi() {
    write(0x0100 + sp, (pc >> 8) & 0x00FF);
    sp--;
    write(0x0100 + sp, pc & 0x00FF);
    sp--;

    setFlag(flags::Break, false);
    setFlag(flags::Unused, true);
    setFlag(flags::Interrupts, true);
    write(0x0100 + sp, status);
    sp--;

    // reads from different address
    address_abs = 0xFFFA;
    uint16_t low = read(address_abs + 0);
    uint16_t high = read(address_abs + 1);
    pc = (high << 8) | low;

    cycles = 8;
}

uint8_t CPU::fetch() {
    if (lookup[current_opcode].addressmode != address_mode::IMP) {
        fetched_data = read(address_abs);
    }
    return fetched_data;
}

// Used for instructions in opcodes
void CPU::branchIf(bool condition) {
    if (condition) {
        cycles++;
        address_abs = pc + address_rel;
        if ((address_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = address_abs;
    }
}

void CPU::compare(uint8_t& reg) {
    fetch();
    store = static_cast<uint16_t>(reg) - static_cast<uint16_t>(fetched_data);
    setFlag(flags::Carry, reg >= fetched_data);
    setFlag(flags::Zero, (store & 0x00FF) == 0x0000);
    setFlag(flags::Negative, store & 0x0080);
}

uint8_t CPU::load(uint8_t& reg) {
    fetch();
    reg = fetched_data;
    setFlag(flags::Zero, reg == 0x00);
    setFlag(flags::Negative, reg & 0x80);
    return 1;
}