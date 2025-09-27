#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <unistd.h> 
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>         
#include <algorithm>   

using namespace std;

// Các hằng số
const int CONSOLE_WIDTH = 120;
const int CONSOLE_HEIGHT = 36;
const string PROJECT_NAME = "\\_____ GIẢI ĐẤU CỜ TƯỚNG VÔ ĐỊCH THẾ GIỚI 2025 _____/";
const string TITLE_TEACHER = "Giảng Viên Hướng Dẫn";
const string TEACHER = "Trần Hồ Thủy Tiên";
const string TEAM_MEMBER = "Sinh viên thực hiện";
const string TEAM_MEMBERS1 = "Lê Minh Mạnh";
const string TEAM_MEMBERS2 = "Nguyễn Thái Pháp";
const string TEAM_MEMBERS3 = "Trần Văn Quyết";
const string TEAM_MEMBERS4 = "Lê Trần Duy Tân";

// Struct lưu thông tin trận đấu
struct TranDau {
    string tenVDV1;     // Tên VĐV thứ nhất
    string tenVDV2;     // Tên VĐV thứ hai
    int idVDV1;         // ID gốc của VĐV thứ nhất
    int idVDV2;         // ID gốc của VĐV thứ hai
    bool ketQua;        // true: VĐV1 thắng, false: VĐV2 thắng
    int vongDau;        // Vòng đấu diễn ra trận này
};

// Struct lưu thông tin vận động viên
struct Player {
    string name;                    // Tên VĐV
    int danhSachGoc;                // ID gốc không đổi
    bool daThang = false;           // Kết quả trận gần nhất
    bool daBiLoai = false;          // Đã bị loại khỏi giải
    int soTranThang = 0;            // Số trận thắng
    int soTranThua = 0;             // Số trận thua
    int diem = 0;                   // Tổng điểm
    int xepHang = 0;                // Xếp hạng cuối cùng
    int vongBiLoai = 0;             // Vòng bị loại
    vector<bool> lichSuThang;       // Lịch sử thắng/thua các trận
    vector<string> doiThuDaGap;     // Danh sách đối thủ đã đấu
};

// Các biến toàn cục
vector<Player> danhSachVDV;                // Danh sách VĐV hiện tại
vector<Player> danhSachBiLoai;             // Danh sách VĐV đã bị loại
vector<vector<TranDau>> lichSuTranDau;     // Lịch sử các trận đã đấu
vector<vector<TranDau>> cacCapDauHienTai;  // Các cặp đấu hiện tại
int soTranDaDau = 0;                       // Số trận đã đấu
int soTranToiDa = 0;                       // Số trận tối đa
bool daNhapDanhSach = false;               // Đã nhập danh sách VĐV chưa
bool daXepCapTiepTheo = false;             // Đã xếp cặp cho trận tiếp theo
bool daQuaVong3 = false;                   // Đã qua vòng 3 chưa
bool daXacDinhVoDich = false;              // Đã xác định vô địch chưa
bool daLoaiVDVTaiVongHienTai = false;      // Đã loại VĐV ở vòng hiện tại

// Khai báo các hàm
void ToaDo(int x, int y);
void DoiMau(int color);
void VeKhung(int x, int y, int w, int h, int color);
void XoaVung(int x, int y, int width, int height);
void VeChuChampions(int x, int y, int color);
void VeCupVang(int a, int b);
void VeHeaderFooter();
void NhapDanhSachNguoiChoi();
void HienThiDanhSachVDV();
void ResetDanhSach();
void XepCapTranDauNgauNhien();
void HienThiCacCapDau();
void NhapKetQuaTranDau();
void HienThiBangXepHang();
void LoaiVanDongVien();
void HienThiDanhSachLoai();
void XacDinhKetQuaCuoiCung();
void HienThiLichSuTranDau();
void VeMenuChinh(int LuaChon);
int MenuChinh();

// Hàm main
int main() {
    system(("mode " + to_string(CONSOLE_WIDTH) + "," + to_string(CONSOLE_HEIGHT)).c_str());
    system("title Giai Dau Co Tuong - Do An Tin Hoc");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    while (true) {
        int chon = MenuChinh();
        switch(chon) {
            case 1: 
                if (!daNhapDanhSach) {
                    NhapDanhSachNguoiChoi();
                } else {
                    HienThiDanhSachVDV();
                }
                break;
            case 2:
                XepCapTranDauNgauNhien();
                break;
            case 3:
                NhapKetQuaTranDau();
                break;
            case 4:
                HienThiBangXepHang();
                break;
            case 5:
                LoaiVanDongVien();
                break;
            case 6:
                HienThiLichSuTranDau();
                break;
            case 7:
                return 0;
            default:
                system("cls");
                cout << "(Chuc nang dang duoc phat trien...)";
                sleep(1);
        }
    }
    return 0;
}

// Các hàm tiện ích giao diện
void ToaDo(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void DoiMau(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void VeKhung(int x, int y, int w, int h, int color) {
    DoiMau(color);
    for (int i = 0; i <= w; i++) {
        ToaDo(x + i, y);
        cout << char(223);
        ToaDo(x + i, y + h);
        cout << char(223);
    }
    for (int j = 0; j <= h; j++) {
        ToaDo(x, y + j);
        cout << char(219);
        ToaDo(x + w, y + j);
        cout << char(219);
    }
    DoiMau(7);
}

void XoaVung(int x, int y, int width, int height) {
    string space(width, ' ');
    for (int i = 0; i < height; i++) {
        ToaDo(x, y + i);
        cout << space;
    }
}

void VeChuChampions(int x, int y, int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD currentColor = consoleInfo.wAttributes;

    DoiMau(color);
    const string championsArt[] = {
        " ██████╗██╗  ██╗ █████╗ ███╗   ███╗██████╗ ██╗ ██████╗ ███╗   ██╗███████╗",
        "██╔════╝██║  ██║██╔══██╗████╗ ████║██╔══██╗██║██╔═══██╗████╗  ██║██╔════╝",
        "██║     ███████║███████║██╔████╔██║██████╔╝██║██║   ██║██╔██╗ ██║███████╗",
        "██║     ██╔══██║██╔══██║██║╚██╔╝██║██╔═══╝ ██║██║   ██║██║╚██╗██║╚════██║",
        "╚██████╗██║  ██║██║  ██║██║ ╚═╝ ██║██║     ██║╚██████╔╝██║ ╚████║███████║",
        " ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝",
        "                                                                         "
    };

    for (int i = 0; i < 7; i++) {
        ToaDo(x, y + i);
        cout << championsArt[i];
    }
    SetConsoleTextAttribute(hConsole, currentColor);
}

void VeCupVang(int a, int b) {
    ToaDo(a, b);
    DoiMau(14);
    printf("            *******");
    ToaDo(a, b+1);
    printf("    ^ ^   ***********   ^ ^");
    ToaDo(a, b+2);
    printf(" ^     ^***************^     ^");
    ToaDo(a, b+3);
    printf("^      *****************      ^");
    ToaDo(a, b+4);
    printf(" ^    *******************    ^");
    ToaDo(a, b+5);
    printf("  ^    *****************    ^");
    ToaDo(a, b+6);
    printf("    ^   ***************   ^");
    ToaDo(a, b+7);
    printf("      ^  *************  ^");
    ToaDo(a, b+8);
    printf("        ^ *********** ^");
    ToaDo(a, b+9);
    printf("           ^*******^");
    ToaDo(a, b+10);
    printf("             *****");
    ToaDo(a, b+11);
    printf("             *****");
    ToaDo(a, b+12);
    printf("             *****");
    ToaDo(a, b+13);
    printf("             *****");
    ToaDo(a, b+14);
    printf("             *****");
    ToaDo(a, b+15);
    printf("            *******");
    ToaDo(a, b+16);
    printf("         *************");
    ToaDo(a, b+17);
    printf("        ****************");
    ToaDo(a, b+18);
    printf("       ******************");
    ToaDo(a, b+19);    
    printf("       ******************");
    ToaDo(a, b+20);    
    printf("       ******************");
    ToaDo(a, b+21);     
    printf("       ******************");
    ToaDo(a, b+22);
    printf("       ******************");
}

void VeHeaderFooter() {
    // Vẽ header
    DoiMau(11);
    ToaDo(0, 0);
    for (int i = 0; i < CONSOLE_WIDTH; i++) cout << "═";
    
    DoiMau(12);
    ToaDo(37, 1);
    cout << PROJECT_NAME;
    
    // Vẽ thông tin giáo viên và nhóm
    DoiMau(8);
    ToaDo(2, 2);
    cout << TITLE_TEACHER;
    ToaDo(CONSOLE_WIDTH - TEAM_MEMBER.length() + 4, 2);
    cout << TEAM_MEMBER;
    ToaDo(3, 3);
    cout << TEACHER;
    ToaDo(CONSOLE_WIDTH - TEAM_MEMBERS1.length() - 2, 3);
    cout << TEAM_MEMBERS1;
    ToaDo(CONSOLE_WIDTH - TEAM_MEMBERS2.length() + 1, 4);
    cout << TEAM_MEMBERS2;
    ToaDo(CONSOLE_WIDTH - TEAM_MEMBERS3.length() + 1, 5);
    cout << TEAM_MEMBERS3;
    ToaDo(CONSOLE_WIDTH - TEAM_MEMBERS4.length() + 0, 6);
    cout << TEAM_MEMBERS4;

    VeChuChampions((CONSOLE_WIDTH - 70)/2, 5, 14);
    
    // Vẽ footer
    DoiMau(11);
    ToaDo(0, CONSOLE_HEIGHT - 2);
    for (int i = 0; i < CONSOLE_WIDTH; i++) cout << "═";
    
    DoiMau(8);
    ToaDo(2, CONSOLE_HEIGHT - 1);
    cout << "Số VĐV: " << danhSachVDV.size() 
        << " | Số trận: " << soTranDaDau << " | Trạng thái: ";
    
    if (danhSachVDV.empty()) {
        cout << "Chưa nhập danh sách VĐV";
    } else {
        cout << "Đang thi đấu";
        if (!lichSuTranDau.empty()) {
            cout << " (" << lichSuTranDau.size() << " trận)";
        }
    }
    
    DoiMau(7);
}

// Các hàm chức năng chính
void NhapDanhSachNguoiChoi() {
    system("cls");
    VeKhung(30, 1, 60, 18, 14);
    DoiMau(11);
    ToaDo(45, 2);
    cout << "╔════ NHAP DANH SACH VAN DONG VIEN ════╗";
    
    int luaChon = 1;
    bool selected = false;
    
    while (!selected) {
        VeKhung(38, 6, 44, 5, 11);
        
        string options[] = {
            "╔══ NHAP TU BAN PHIM ══╗",
            "╔══ DOC TU FILES TXT ══╗"
        };
        
        for (int i = 0; i < 2; i++) {
            ToaDo(52, 4 + i*3);
            if (luaChon == i+1) {
                DoiMau(14);
                cout << options[i];
                ToaDo(52, 5 + i*3);
                cout << "╚══════════════════════╝";
            } else {
                DoiMau(8);
                cout << options[i];
                ToaDo(52, 5 + i*3);
                cout << "╚══════════════════════╝";
            }
        }
        
        ToaDo(43, 12);
        DoiMau(8);
        cout << "▲ ▼ : Di chuyen   ENTER : Chon   ESC : Thoat";
        DoiMau(7);
        
        char ch = getch();
        if (ch == 72) luaChon = (luaChon == 1) ? 2 : 1;
        else if (ch == 80) luaChon = (luaChon == 2) ? 1 : 2;
        else if (ch == 13) selected = true;
        else if (ch == 27) return;
    }

    danhSachVDV.clear();

    if (luaChon == 1) {
        int soLuong;
        bool nhapHopLe = false;
        do {
            system("cls");
            VeKhung(30, 3, 60, 6, 11);
            DoiMau(14);
            ToaDo(45, 4);
            cout << "╔══ NHAP SO LUONG VAN DONG VIEN ══╗";
            DoiMau(15);
            
            ToaDo(42, 7);
            cout << "┌──────────────────────────────────────┐";
            ToaDo(42, 8);
            cout << "│ So VDV (phai la so chan):            │";
            ToaDo(42, 9);
            cout << "└──────────────────────────────────────┘";
            ToaDo(70, 8);
            string input;
            cin >> input;

            bool laSo = true;
            for (char c : input) {
                if (!isdigit(c)) {
                    laSo = false;
                    break;
                }
            }
            if (laSo) {
                soLuong = stoi(input);
                if (soLuong % 2 != 0 || soLuong <= 4) {
                    ToaDo(38, 11);
                    DoiMau(12);
                    cout << "⚠ Loi: So VDV phai la so chan và lon hon 4! Vui long nhap lai.";
                    usleep(1500000);
                    XoaVung(38, 11, 40, 1);
                } else {
                    nhapHopLe = true;
                }
            } else {
                ToaDo(38, 11);
                DoiMau(12);
                cout << "⚠ Loi: Chi duoc nhap so! Vui long nhap lai.";
                usleep(1500000);
                XoaVung(38, 11, 40, 1);
                XoaVung(70, 8, 10, 1);
            }

        } while (soLuong % 2 != 0 || soLuong <= 4);
        cin.ignore();

        system("cls");
        VeKhung(20, 2, 80, soLuong + 4, 11);
        DoiMau(14);
        ToaDo(45, 3);
        cout << "╔══ NHAP TEN VAN DONG VIEN ══╗";
        DoiMau(15);
        
        for (int i = 0; i < soLuong; ++i) {
            Player p;
            ToaDo(42, 6 + i);
            cout << "╔ VDV " << i + 1 << ": ";
            ToaDo(42, 7 + i);
            cout << "╚═══════════════════════════════════════";
            ToaDo(51, 6 + i);
            getline(cin, p.name);
            p.danhSachGoc = i;
            danhSachVDV.push_back(p);
        }
    } else {
        system("cls");
        VeKhung(30, 3, 60, 6, 11);
        DoiMau(14);
        ToaDo(45, 4);
        cout << "╔══ DOC DANH SACH TU FILE ══╗";
        DoiMau(15);
        
        string tenFile;
        ToaDo(35, 7);
        cout << "┌───────────────────────────────────────────────────────┐";
        ToaDo(35, 8);
        cout << "│ Nhap ten file (vd: danhsach.txt):                     │";
        ToaDo(35, 9);
        cout << "└───────────────────────────────────────────────────────┘";
        ToaDo(71, 8);
        cin >> tenFile;
        cin.ignore();
        
        FILE* file = fopen(tenFile.c_str(), "r");
        while (!file) {
            ToaDo(35, 11);
            DoiMau(12);
            cout << "⚠ Loi: Khong the mo file! Vui long nhap lai: ";
            DoiMau(15);
            ToaDo(80, 11);
            cin >> tenFile;
            file = fopen(tenFile.c_str(), "r");
        }

        system("cls");
        VeKhung(40, 5, 40, 4, 11);
        DoiMau(14);
        ToaDo(50, 6);
        cout << "DANG DOC DU LIEU...";
        DoiMau(15);
        
        char line[100];
        int count = 0;
        while (fgets(line, sizeof(line), file)) {
            if (strlen(line) <= 1) continue;
            Player p;
            line[strcspn(line, "\n")] = 0;
            p.name = line;
            p.danhSachGoc = count;
            danhSachVDV.push_back(p);
            ++count;
            
            ToaDo(45, 8);
            cout << "Da doc " << count << " VDV";
            Sleep(50);
        }
        fclose(file);

        if (count % 2 != 0) {
            system("cls");
            VeKhung(35, 5, 50, 4, 12);
            DoiMau(12);
            ToaDo(40, 7);
            cout << "⚠ Loi: So VDV trong file khong phai so chan!";
            ToaDo(49, 8);
            cout << "Khong the tiep tuc thi dau.         ";
            danhSachVDV.clear();
            Sleep(3000);
            return;
        }
    }

    do {
        system("cls");
        VeKhung(30, 3, 60, 6, 11);
        DoiMau(14);
        ToaDo(45, 4);
        cout << "╔══ NHAP SO TRAN DAU TOI DA ══╗";
        DoiMau(15);
        
        ToaDo(42, 7);
        cout << "┌──────────────────────────────────────┐";
        ToaDo(42, 8);
        cout << "│ So tran dau (>3):                    │";
        ToaDo(42, 9);
        cout << "└──────────────────────────────────────┘";
        ToaDo(70, 8);
        cin >> soTranToiDa;
        
        if (soTranToiDa <= 3) {
            ToaDo(38, 11);
            DoiMau(12);
            cout << "⚠ Loi: So tran dau phai > 3! Vui long nhap lai.";
            usleep(1500000);
        }
    } while (soTranToiDa <= 3);

    daNhapDanhSach = true;
    
    // Hiển thị danh sách VĐV sau khi nhập xong
    HienThiDanhSachVDV();
}

void HienThiDanhSachVDV() {
    system("cls");
    VeKhung(20, 2, 80, danhSachVDV.size() + 7, 11);
    DoiMau(14);
    ToaDo(45, 1);
    cout << "╔══ DANH SACH VAN DONG VIEN ══╗";
    
    DoiMau(15);
    for (int i = 0; i < danhSachVDV.size(); ++i) {
        ToaDo(47, 3 + i);
        cout << i + 1 << ". " << danhSachVDV[i].name;
    }
    
    // Hiển thị hướng dẫn
    ToaDo(40, 6 + danhSachVDV.size());
    DoiMau(8);
    cout << "HUONG DAN:";
    ToaDo(40, 7 + danhSachVDV.size());
    cout << "[R] Reset danh sach";
    ToaDo(40, 8 + danhSachVDV.size());
    cout << "[1] Quay ve menu chinh";
    ToaDo(40, 9 + danhSachVDV.size());
    cout << "[2] Xep cap tran dau";
    
    // Xử lý lựa chọn
    while (true) {
        char ch = getch();
        if (ch == 'r' || ch == 'R') {
            ResetDanhSach();
            return;
        } else if (ch == '1') {
            return;
        } else if (ch == '2') {
            XepCapTranDauNgauNhien();
            return;
        }
    }
}

void ResetDanhSach() {
    system("cls");
    VeKhung(35, 5, 50, 4, 12);
    DoiMau(12);
    ToaDo(40, 6);
    cout << "BAN CO CHAC MUON XOA TOAN BO DANH SACH?";
    
    int luaChon = 1;
    bool selected = false;
    string options[] = {"╔══ CO ══╗", "╔══ KHONG ══╗"};
    
    while (!selected) {
        for (int i = 0; i < 2; i++) {
            ToaDo(45 + i*15, 8);
            if (luaChon == i+1) {
                DoiMau(12);
                cout << options[i];
            } else {
                DoiMau(8);
                cout << options[i];
            }
        }
        
        char ch = getch();
        if (ch == 75) luaChon = 1;
        else if (ch == 77) luaChon = 2;
        else if (ch == 13) selected = true;
    }
    
    if (luaChon == 1) {
        danhSachVDV.clear();
        danhSachBiLoai.clear();
        lichSuTranDau.clear();
        cacCapDauHienTai.clear();
        daNhapDanhSach = false;
        soTranDaDau = 0;
        soTranToiDa = 0;
        daXacDinhVoDich = false;
        
        // Hiển thị thông báo và quay về menu
        system("cls");
        VeKhung(35, 5, 50, 4, 10);
        DoiMau(10);
        ToaDo(45, 7);
        cout << "DA XOA TOAN BO DANH SACH!";
        sleep(2);
    } else {
        // Nếu không reset, hiển thị lại danh sách
        if (!danhSachVDV.empty()) {
            HienThiDanhSachVDV();
        }
    }
}

void XepCapTranDauNgauNhien() {
    if (danhSachVDV.empty()) {
        system("cls");
        VeKhung(35, 10, 50, 3, 12);
        DoiMau(12);
        ToaDo(40, 11);
        cout << "Chua co danh sach VDV. Vui long chay muc 1 truoc!";
        sleep(2);
        return;
    }

    // Kiểm tra nếu đã xác định được nhà vô địch
    if (daXacDinhVoDich) {
        system("cls");
        VeKhung(35, 10, 50, 3, 12);
        DoiMau(12);
        ToaDo(40, 11);
        cout << "Da Xac Dinh Duoc Nha Vo Dich!";
        sleep(2);
        return;
    }

    // Tạo danh sách VĐV còn lại (chưa bị loại)
    vector<int> danhSachConLai;
    for (int i = 0; i < danhSachVDV.size(); i++) {
        if (!danhSachVDV[i].daBiLoai) {
            danhSachConLai.push_back(i);
        }
    }

    // Xử lý trận cuối cùng khi còn 4 VĐV
    if (danhSachConLai.size() == 4 && soTranDaDau == soTranToiDa - 1) {
        // Sắp xếp theo điểm số để xác định thứ tự
        sort(danhSachConLai.begin(), danhSachConLai.end(), [&](int a, int b) {
            const Player& p1 = danhSachVDV[a];
            const Player& p2 = danhSachVDV[b];
            if (p1.diem != p2.diem) return p1.diem > p2.diem;
            if (p1.soTranThang != p2.soTranThang) return p1.soTranThang > p2.soTranThang;
            return p1.soTranThua < p2.soTranThua;
        });

        // Tạo các cặp đấu
        cacCapDauHienTai.clear();
        cacCapDauHienTai.resize(1);
        
        // Tạo trận chung kết
        TranDau chungKet;
        chungKet.tenVDV1 = danhSachVDV[danhSachConLai[0]].name;
        chungKet.tenVDV2 = danhSachVDV[danhSachConLai[1]].name;
        chungKet.idVDV1 = danhSachVDV[danhSachConLai[0]].danhSachGoc;
        chungKet.idVDV2 = danhSachVDV[danhSachConLai[1]].danhSachGoc;
        chungKet.ketQua = false;
        chungKet.vongDau = soTranDaDau + 1;
        cacCapDauHienTai[0].push_back(chungKet);

        // Tạo trận tranh hạng ba
        TranDau tranhHangBa;
        tranhHangBa.tenVDV1 = danhSachVDV[danhSachConLai[2]].name;
        tranhHangBa.tenVDV2 = danhSachVDV[danhSachConLai[3]].name;
        tranhHangBa.idVDV1 = danhSachVDV[danhSachConLai[2]].danhSachGoc;
        tranhHangBa.idVDV2 = danhSachVDV[danhSachConLai[3]].danhSachGoc;
        tranhHangBa.ketQua = false;
        tranhHangBa.vongDau = soTranDaDau + 1;
        cacCapDauHienTai[0].push_back(tranhHangBa);

        // Hiển thị thông báo đặc biệt cho trận chung kết
        system("cls");
        VeKhung(20, 5, 80, 10, 14);
        DoiMau(14);
        ToaDo(40, 6);
        cout << "╔════════════ TRAN CHUNG KET ════════════╗";
        ToaDo(40, 7);
        cout << "║                                        ║";
        ToaDo(40, 8);
        cout << "║  " << left << setw(36) << (chungKet.tenVDV1 + " vs " + chungKet.tenVDV2);
        ToaDo(81, 8);
        cout  << "║";
        ToaDo(40, 9);
        cout << "║                                        ║";
        ToaDo(40, 10);
        cout << "╠══════════ TRAN TRANH HANG BA ══════════╣";
        ToaDo(40, 11);
        cout << "║                                        ║";
        ToaDo(40, 12);
        cout << "║  " << left << setw(36) << (tranhHangBa.tenVDV1 + " vs " + tranhHangBa.tenVDV2);
        ToaDo(81, 12);
        cout  << "║";
        ToaDo(40, 13);
        cout << "║                                        ║";
        ToaDo(40, 14);
        cout << "╚════════════════════════════════════════╝";

        ToaDo(40, 16);
        DoiMau(8);
        cout << "NHAN:";
        ToaDo(40, 17);
        cout << "[1] Quay lai menu chinh";
        ToaDo(40, 18);
        cout << "[3] Nhap ket qua cac tran nay";
    
        char ch = getch();
        if (ch == '3') {
            NhapKetQuaTranDau();
        }

        daXepCapTiepTheo = true;
        return;
    }

    if (soTranDaDau >= 3 && !daLoaiVDVTaiVongHienTai) {
        LoaiVanDongVien(); // Gọi hàm loại VĐV trực tiếp
        daLoaiVDVTaiVongHienTai = true;
        return; // Thoát để hiển thị kết quả loại trước
    }

    // Tự động loại VĐV nếu đã qua vòng 3, chưa loại và còn >4 VĐV
    if (soTranDaDau >= 3 && !daLoaiVDVTaiVongHienTai && danhSachConLai.size() > 4) {
        // Tính số VĐV cần loại
        int soVDVConLai = 4;
        int soTranConLai = soTranToiDa - soTranDaDau;
        int soVDVBiLoai = max(2, (int)(danhSachConLai.size() - soVDVConLai) / soTranConLai);
        
        // Đảm bảo số chẵn và không vượt quá
        soVDVBiLoai = min(soVDVBiLoai, (int)danhSachConLai.size() - 4);
        if (soVDVBiLoai % 2 != 0) soVDVBiLoai--;

        if (soVDVBiLoai > 0) {
            // Sắp xếp VĐV theo thành tích (xấu nhất lên đầu)
            sort(danhSachConLai.begin(), danhSachConLai.end(), [&](int a, int b) {
                const Player& p1 = danhSachVDV[a];
                const Player& p2 = danhSachVDV[b];
                if (p1.diem != p2.diem) return p1.diem < p2.diem;
                if (p1.soTranThua != p2.soTranThua) return p1.soTranThua > p2.soTranThua;
                return p1.lichSuThang.size() > 0 && !p1.lichSuThang.back();
            });

            // Đánh dấu loại - KIỂM TRA TRÙNG LẶP TRƯỚC KHI THÊM
            for (int i = 0; i < soVDVBiLoai; i++) {
                int idx = danhSachConLai[i];
                // Kiểm tra xem VĐV đã bị loại chưa
                bool daBiLoaiTruocDo = false;
                for (const auto& vdv : danhSachBiLoai) {
                    if (vdv.name == danhSachVDV[idx].name) {
                        daBiLoaiTruocDo = true;
                        break;
                    }
                }
                
                if (!daBiLoaiTruocDo) {
                    danhSachVDV[idx].daBiLoai = true;
                    danhSachVDV[idx].vongBiLoai = soTranDaDau;
                    danhSachBiLoai.push_back(danhSachVDV[idx]);
                }
            }

            // Cập nhật danh sách
            danhSachConLai.erase(
                remove_if(danhSachConLai.begin(), danhSachConLai.end(), 
                        [&](int idx) { return danhSachVDV[idx].daBiLoai; }),
                danhSachConLai.end()
            );

            // Đánh dấu đã loại
            daLoaiVDVTaiVongHienTai = true;
            
            // Thông báo
            system("cls");
            VeKhung(35, 10, 50, 3, 12);
            DoiMau(12);
            ToaDo(40, 11);
            cout << "DA TU DONG LOAI " << soVDVBiLoai << " VDV!";
            sleep(2);
        }
    }
    
    // Nếu đã xếp cặp từ trước thì chỉ hiển thị
    if (daXepCapTiepTheo && !cacCapDauHienTai.empty()) {
        HienThiCacCapDau();
        return;
    }

    // Reset trạng thái
    daXepCapTiepTheo = false;
    cacCapDauHienTai.clear();
    cacCapDauHienTai.resize(1);

    // Nếu không đủ VĐV để xếp cặp
    if (danhSachConLai.size() < 2) {
        system("cls");
        VeKhung(35, 10, 50, 3, 12);
        DoiMau(12);
        ToaDo(40, 11);
        cout << "KHONG DU VAN DONG VIEN DE XEP CAP DAU!";
        sleep(2);
        return;
    }

    // Sắp xếp VĐV theo số trận thắng giảm dần, số trận thua tăng dần
    sort(danhSachConLai.begin(), danhSachConLai.end(), [&](int a, int b) {
        const Player& p1 = danhSachVDV[a];
        const Player& p2 = danhSachVDV[b];
        if (p1.soTranThang != p2.soTranThang) return p1.soTranThang > p2.soTranThang;
        return p1.soTranThua < p2.soTranThua;
    });

    // Nhóm VĐV theo số trận thắng
    map<int, vector<int>> nhomTheoTrangThai;
    for (int idx : danhSachConLai) {
        nhomTheoTrangThai[danhSachVDV[idx].soTranThang].push_back(idx);
    }

    // Xếp cặp theo nhóm
    vector<pair<int, int>> cacCapDau;
    vector<int> doiLe;

    for (auto& nhom : nhomTheoTrangThai) {
        auto& dsVDV = nhom.second;
        
        // Xáo trộn ngẫu nhiên trong nhóm
        random_shuffle(dsVDV.begin(), dsVDV.end());

        // Xử lý VĐV lẻ từ nhóm trước (nếu có)
        if (!doiLe.empty() && !dsVDV.empty()) {
            dsVDV.push_back(doiLe.back());
            doiLe.pop_back();
            random_shuffle(dsVDV.begin(), dsVDV.end());
        }

        // Tạo cặp trong nhóm
        for (int i = 0; i < dsVDV.size(); i += 2) {
            if (i + 1 < dsVDV.size()) {
                cacCapDau.push_back({dsVDV[i], dsVDV[i+1]});
            } else {
                doiLe.push_back(dsVDV[i]);
            }
        }
    }

    // Xử lý VĐV lẻ còn lại
    while (!doiLe.empty()) {
        int vdvLe = doiLe.back();
        doiLe.pop_back();

        // Tìm VĐV có số trận thắng thấp hơn 1 bậc
        int targetWins = danhSachVDV[vdvLe].soTranThang - 1;
        bool timThay = false;

        if (nhomTheoTrangThai.count(targetWins)) {
            auto& nhom = nhomTheoTrangThai[targetWins];
            if (!nhom.empty()) {
                int vdvThapHon = nhom.back();
                nhom.pop_back();
                cacCapDau.push_back({vdvLe, vdvThapHon});
                timThay = true;
            }
        }

        if (!timThay) {
            ToaDo(40, 4 + cacCapDau.size());
            cout << "VDV " << danhSachVDV[vdvLe].name << " duoc nghi vong nay";
        }
    }

    // Sắp xếp các cặp đấu theo trình tự
    sort(cacCapDau.begin(), cacCapDau.end(), [&](const pair<int, int>& a, const pair<int, int>& b) {
        int aWins = min(danhSachVDV[a.first].soTranThang, danhSachVDV[a.second].soTranThang);
        int bWins = min(danhSachVDV[b.first].soTranThang, danhSachVDV[b.second].soTranThang);
        return aWins > bWins;
    });

    // Thêm tất cả các cặp vào một nhóm duy nhất (không phân bàn)
    for (const auto& cap : cacCapDau) {
        TranDau td;
        td.tenVDV1 = danhSachVDV[cap.first].name;
        td.tenVDV2 = danhSachVDV[cap.second].name;
        td.idVDV1 = danhSachVDV[cap.first].danhSachGoc;
        td.idVDV2 = danhSachVDV[cap.second].danhSachGoc;
        td.ketQua = false;
        td.vongDau = soTranDaDau + 1;
        cacCapDauHienTai[0].push_back(td);
    }

    daXepCapTiepTheo = true;
    HienThiCacCapDau();
}

void HienThiCacCapDau() {
    system("cls");
    VeKhung(10, 2, 100, cacCapDauHienTai[0].size() + 10, 11);
    
    DoiMau(14);
    ToaDo(45, 1);
    cout << "| DANH SACH THI DAU TRAN " << soTranDaDau + 1 << "/" << soTranToiDa << " |";
    DoiMau(15);
    
    int currentLine = 3;
    for (size_t i = 0; i < cacCapDauHienTai[0].size(); i++) {
        ToaDo(30, currentLine);
        cout << "╔═══════════════════════════════════════════════════════╗";
    
        ToaDo(30, currentLine + 1);
        cout << "║ Ban " << i + 1 << ":" << cacCapDauHienTai[0][i].tenVDV1
                         << " vs " << cacCapDauHienTai[0][i].tenVDV2;
        ToaDo(86, currentLine + 1);
        cout << "║";                           
    
        ToaDo(30, currentLine + 2);
        cout << "╚═══════════════════════════════════════════════════════╝";
    
        currentLine += 3;
    }

    daXepCapTiepTheo = false;
    
    ToaDo(40, currentLine);
    DoiMau(8);
    cout << "NHAN:";
    ToaDo(40, currentLine + 1);
    cout << "[1] Quay lai menu chinh";
    ToaDo(40, currentLine + 2);
    cout << "[3] Nhap ket qua cac tran nay";
    
    char ch = getch();
    if (ch == '3') {
        NhapKetQuaTranDau();
    }
}

void NhapKetQuaTranDau() {
    if (danhSachVDV.empty()) {
        system("cls");
        VeKhung(35, 10, 50, 3, 12);
        DoiMau(12);
        ToaDo(40, 11);
        cout << "Chua co danh sach VDV. Vui long chay muc 1 truoc!";
        sleep(3);  
        return;    
    }

    if (cacCapDauHienTai.empty() || cacCapDauHienTai[0].empty()) {
        system("cls");
        VeKhung(35, 10, 50, 3, 12);
        DoiMau(12);
        ToaDo(40, 11);
        cout << "Chua co cap thi dau nao. Vui long chay muc 2 truoc!";
        sleep(3);  
        return;    
    } 
    
    if (soTranDaDau >= soTranToiDa) {
        system("cls");
        VeKhung(35, 10, 50, 3, 12);
        DoiMau(12);
        ToaDo(40, 11);
        cout << "Giai dau da ket thuc!";
        sleep(3);  
        return;    
    }

    vector<TranDau> cacCapDauHienTaiBackup = cacCapDauHienTai[0];

    system("cls");
    VeKhung(10, 2, 100, cacCapDauHienTaiBackup.size() + 8, 11);
    DoiMau(14);
    ToaDo(45, 3);
    cout << "| NHAP KET QUA TRAN DAU THU " << soTranDaDau + 1 << "/" << soTranToiDa << " |";
    DoiMau(15);

    bool isTranCuoiCung = false;
    vector<int> danhSachConLai;
    for (int i = 0; i < danhSachVDV.size(); i++) {
        if (!danhSachVDV[i].daBiLoai) {
            danhSachConLai.push_back(i);
        }
    }
    isTranCuoiCung = (danhSachConLai.size() == 4 && soTranDaDau == soTranToiDa - 1);

    for (size_t i = 0; i < cacCapDauHienTaiBackup.size(); i++) {
        string input;
        int ketqua = 0;
        bool validInput = false;

        while (!validInput) {
            VeKhung(15, 5 + i*2, 90, 2, 11);
            ToaDo(20, 6 + i*2);
            
            if (isTranCuoiCung) {
                if (i == 0) {
                    cout << "CHUNG KET: " << cacCapDauHienTaiBackup[i].tenVDV1 << " vs " << cacCapDauHienTaiBackup[i].tenVDV2;
                } else {
                    cout << "TRANH HANG BA: " << cacCapDauHienTaiBackup[i].tenVDV1 << " vs " << cacCapDauHienTaiBackup[i].tenVDV2;
                }
            } else {
                cout << "Ban " << i + 1 << ": " << cacCapDauHienTaiBackup[i].tenVDV1 << " vs " << cacCapDauHienTaiBackup[i].tenVDV2;
            }
            
            ToaDo(20, 7 + i*2);
            cout << "Nhap ket qua (1: " << cacCapDauHienTaiBackup[i].tenVDV1 << " thang, 2: " << cacCapDauHienTaiBackup[i].tenVDV2 << " thang): ";
            
            cin >> input;
            
            if (input.size() == 1 && (input[0] == '1' || input[0] == '2')) {
                ketqua = input[0] - '0';
                validInput = true;
            } else {
                ToaDo(20, 8 + i*2);
                DoiMau(12);
                cout << "Nhap khong chinh xac! Chi nhap 1 hoac 2.";
                DoiMau(15);
                usleep(1500000);
                
                ToaDo(20, 8 + i*2);
                cout << "                                                      ";
                
                ToaDo(20 + 50, 7 + i*2);
                cout << "   ";
            }
        }

        // Cập nhật kết quả vào bản sao
        cacCapDauHienTaiBackup[i].ketQua = (ketqua == 1);

        // Tìm và cập nhật kết quả cho từng VĐV
        for (auto& vdv : danhSachVDV) {
            if (vdv.danhSachGoc == cacCapDauHienTaiBackup[i].idVDV1) {
                if (ketqua == 1) {
                    vdv.daThang = true;
                    vdv.soTranThang++;
                    vdv.diem += 3;
                    vdv.lichSuThang.push_back(true);
                    
                    if (isTranCuoiCung && i == 0) {
                        vdv.xepHang = 1;
                    } else if (isTranCuoiCung && i == 1) {
                        vdv.xepHang = 3;
                    }
                } else {
                    vdv.daThang = false;
                    vdv.soTranThua++;
                    vdv.lichSuThang.push_back(false);
                    
                    if (isTranCuoiCung && i == 0) {
                        vdv.xepHang = 2;
                    } else if (isTranCuoiCung && i == 1) {
                        vdv.xepHang = 4;
                    }
                }
                vdv.doiThuDaGap.push_back(cacCapDauHienTaiBackup[i].tenVDV2);
            }
            
            if (vdv.danhSachGoc == cacCapDauHienTaiBackup[i].idVDV2) {
                if (ketqua == 2) {
                    vdv.daThang = true;
                    vdv.soTranThang++;
                    vdv.diem += 3;
                    vdv.lichSuThang.push_back(true);
                    
                    if (isTranCuoiCung && i == 0) {
                        vdv.xepHang = 1;
                    } else if (isTranCuoiCung && i == 1) {
                        vdv.xepHang = 3;
                    }
                } else {
                    vdv.daThang = false;
                    vdv.soTranThua++;
                    vdv.lichSuThang.push_back(false);
                    
                    if (isTranCuoiCung && i == 0) {
                        vdv.xepHang = 2;
                    } else if (isTranCuoiCung && i == 1) {
                        vdv.xepHang = 4;
                    }
                }
                vdv.doiThuDaGap.push_back(cacCapDauHienTaiBackup[i].tenVDV1);
            }
        }
    }

    soTranDaDau++;
    daLoaiVDVTaiVongHienTai = false;

    if (soTranDaDau >= 3) {
        daQuaVong3 = true;
    }
    
    lichSuTranDau.push_back(cacCapDauHienTaiBackup);
    cacCapDauHienTai.clear();
    
    if (isTranCuoiCung) {
        daXacDinhVoDich = true;
        XacDinhKetQuaCuoiCung();
        return;
    }
    
    system("cls");
    VeKhung(20, 5, 80, 10, 14);
    DoiMau(14);
    ToaDo(45, 6);
    cout << "DA LUU KET QUA TRAN DAU THANH CONG!";
    
    ToaDo(40, 17);
    DoiMau(8);
    cout << "NHAN:";
    ToaDo(40, 18);
    cout << "[1] Quay lai menu chinh";
    
    if (!isTranCuoiCung) {
        ToaDo(40, 19);
        cout << "[2] Xep cap cho tran tiep theo";
    }
    
    ToaDo(40, 20);
    cout << "[4] Xem bang xep hang";
    
    char ch = getch();
    if (ch == '2' && !isTranCuoiCung) {
        XepCapTranDauNgauNhien();
    } else if (ch == '4') {
        HienThiBangXepHang();
    }
}

void HienThiBangXepHang() {
    if (danhSachVDV.empty()) {
        system("cls");
        VeKhung(35, 10, 50, 3, 12);
        DoiMau(12);
        ToaDo(40, 11);
        cout << "Chua co danh sach VDV. Vui long chay muc 1 truoc!";
        sleep(3);
        return;
    }

    if (daXacDinhVoDich) {
        XacDinhKetQuaCuoiCung();
        return;
    }

    system("cls");
    
    vector<Player> danhSachConLai;
    for (const auto& vdv : danhSachVDV) {
        if (!vdv.daBiLoai) {
            danhSachConLai.push_back(vdv);
        }
    }

    auto tinhChuoiThua = [](const Player& p) {
        int chuoiThua = 0;
        for (auto it = p.lichSuThang.rbegin(); it != p.lichSuThang.rend(); ++it) {
            if (*it == false) {
                chuoiThua++;
            } else {
                break;
            }
        }
        return chuoiThua;
    };

    sort(danhSachConLai.begin(), danhSachConLai.end(), [&](const Player& a, const Player& b) {
        if (a.diem != b.diem) return a.diem > b.diem;
        if (a.soTranThang != b.soTranThang) return a.soTranThang > b.soTranThang;
        return a.soTranThua < b.soTranThua;
    });

    int soDong = danhSachConLai.size() + 10;
    VeKhung(10, 2, 100, soDong, 11);
    
    DoiMau(14);
    ToaDo(45, 3);
    cout << "| BANG XEP HANG SAU TRAN " << soTranDaDau << " |";
    DoiMau(15);

    ToaDo(15, 5);
    cout << "STT";
    ToaDo(25, 5);
    cout << "HO VA TEN";
    ToaDo(55, 5);
    cout << "TRAN THANG";
    ToaDo(70, 5);
    cout << "TRAN THUA";
    ToaDo(85, 5);
    cout << "CHUOI THUA";

    for (int i = 0; i < danhSachConLai.size(); ++i) {
        ToaDo(15, 7 + i);
        cout << i + 1;
        ToaDo(25, 7 + i);
        cout << danhSachConLai[i].name;
        ToaDo(55, 7 + i);
        cout << danhSachConLai[i].soTranThang;
        ToaDo(70, 7 + i);
        cout << danhSachConLai[i].soTranThua;
        ToaDo(85, 7 + i);
        cout << tinhChuoiThua(danhSachConLai[i]);
        DoiMau(15);
        
        usleep(50000);
    }

    DoiMau(8);
    ToaDo(40, 9 + danhSachConLai.size());
    cout << "NHAN:";
    ToaDo(40, 10 + danhSachConLai.size());
    cout << "[1] Quay lai menu chinh";
    ToaDo(40, 11 + danhSachConLai.size());
    cout << "[2] Xep cap tran tiep theo";
    
    if (soTranDaDau >= 3 && !daLoaiVDVTaiVongHienTai && danhSachConLai.size() > 4) {
        ToaDo(40, 12 + danhSachConLai.size());
        cout << "[5] Loai VDV co thanh tich thap nhat";
    }

    while (true) {
        char ch = getch();
        if (ch == '1') return;
        else if (ch == '2') {
            XepCapTranDauNgauNhien();
            return;
        }
        else if (ch == '5' && soTranDaDau >= 3 && !daLoaiVDVTaiVongHienTai && danhSachConLai.size() > 4) {
            LoaiVanDongVien();
            return;
        }
    }
}

void LoaiVanDongVien() {
    if (daXacDinhVoDich) {
        system("cls");
        VeKhung(35, 10, 50, 3, 12);
        DoiMau(12);
        ToaDo(40, 11);
        cout << "Giai dau da ket thuc. Khong the loai VDV nua!";
        sleep(2);
        return;
    }
    
    if (soTranDaDau < lichSuTranDau.size()) {
        system("cls");
        VeKhung(35, 10, 50, 3, 12);
        DoiMau(12);
        ToaDo(40, 11);
        cout << "Vui long nhap ket qua tran " << lichSuTranDau.size() + 1 << " truoc khi loai VDV!";
        sleep(2);
        
        if (!danhSachBiLoai.empty()) {
            HienThiDanhSachLoai();
        }
        return;
    }

    if (soTranDaDau < 3 || danhSachVDV.size() <= 4) {
        system("cls");
        VeKhung(35, 10, 50, 3, 12);
        DoiMau(12);
        ToaDo(40, 11);
        cout << "Chi duoc loai VDV tu tran 3 tro di va khi con > 4 VDV!";
        sleep(2);
        
        if (!danhSachBiLoai.empty()) {
            HienThiDanhSachLoai();
        }
        return;
    }

    if (daLoaiVDVTaiVongHienTai) {
        system("cls");
        VeKhung(35, 10, 50, 3, 12);
        DoiMau(12);
        ToaDo(40, 11);
        cout << "DA LOAI VDV O VONG NAY ROI!";
        sleep(2);
        return;
    }

    int soVDVConLai = 4;
    int soTranConLai = soTranToiDa - soTranDaDau;
    int soVDVBiLoai = max(2, (int)(danhSachVDV.size() - soVDVConLai) / soTranConLai);
    
    soVDVBiLoai = min(soVDVBiLoai, (int)danhSachVDV.size() - 4);
    if (soVDVBiLoai % 2 != 0) { 
        soVDVBiLoai--;
        if (soVDVBiLoai < 2) {
            soVDVBiLoai = 2;
        }
    } 

    if (soVDVBiLoai > 0) {
        auto tinhChuoiThua = [](const Player& p) {
            int chuoiThua = 0;
            for (auto it = p.lichSuThang.rbegin(); it != p.lichSuThang.rend(); ++it) {
                if (*it == false) {
                    chuoiThua++;
                } else {
                    break;
                }
            }
            return chuoiThua;
        };

        sort(danhSachVDV.begin(), danhSachVDV.end(), [&](const Player& a, const Player& b) {
            if (a.soTranThua != b.soTranThua) {
                return a.soTranThua > b.soTranThua;
            }
            return tinhChuoiThua(a) > tinhChuoiThua(b);
        });

        for (int i = 0; i < soVDVBiLoai; i++) {
            danhSachVDV[i].daBiLoai = true;
            danhSachVDV[i].vongBiLoai = soTranDaDau;
            bool daTonTai = false;
            for (const auto& vdv : danhSachBiLoai) {
                if (vdv.name == danhSachVDV[i].name) {
                    daTonTai = true;
                    break;
                }
            }
            if (!daTonTai) {
                danhSachBiLoai.push_back(danhSachVDV[i]);
            }
        }

        danhSachVDV.erase(
            remove_if(danhSachVDV.begin(), danhSachVDV.end(), 
                    [](const Player& p) { return p.daBiLoai; }),
            danhSachVDV.end()
        );

        daLoaiVDVTaiVongHienTai = true;
        
        HienThiDanhSachLoai();
    }

    ToaDo(40, 10 + danhSachBiLoai.size());
    DoiMau(8);
    cout << "NHAN:";
    ToaDo(40, 11 + danhSachBiLoai.size());
    cout << "[1] Quay lai menu chinh";
    ToaDo(40, 12 + danhSachBiLoai.size());
    cout << "[2] Xep cap cho tran tiep theo";
    
    char ch = getch();
    if (ch == '2') {
        XepCapTranDauNgauNhien();
    }
}

void HienThiDanhSachLoai() {
    // Loại bỏ trùng lặp trong danhSachBiLoai
    vector<Player> danhSachKhongTrungLap;
    map<string, bool> daXuatHien;
    
    for (const auto& vdv : danhSachBiLoai) {
        if (!daXuatHien[vdv.name]) {
            danhSachKhongTrungLap.push_back(vdv);
            daXuatHien[vdv.name] = true;
        }
    }

    system("cls");
    
    // Tính toán chiều cao khung dựa trên số lượng VĐV
    int soDong = danhSachKhongTrungLap.size() + 10;
    VeKhung(5, 2, 110, soDong, 14);
    
    DoiMau(14);
    ToaDo(45, 3);
    cout << "DANH SACH VDV BI LOAI";
    
    // Độ rộng các cột
    const int col1Width = 5;   // STT
    const int col2Width = 30;  // Họ và tên
    const int col3Width = 15;  // Trận thắng
    const int col4Width = 15;  // Trận thua
    const int col5Width = 15;  // Vòng bị loại

    // Hiển thị tiêu đề cột
    DoiMau(11);
    ToaDo(10, 5);
    cout << left << setw(col1Width) << "STT" 
         << setw(col2Width) << "HO VA TEN" 
         << setw(col3Width) << "TRAN THANG" 
         << setw(col4Width) << "TRAN THUA" 
         << setw(col5Width) << "VONG BI LOAI";

    // Dòng phân cách
    DoiMau(8);
    ToaDo(10, 6);
    cout << string(col1Width + col2Width + col3Width + col4Width + col5Width, '-');

    // Hiển thị dữ liệu
    DoiMau(15);
    for (int i = 0; i < danhSachKhongTrungLap.size(); ++i) {
        ToaDo(10, 7 + i);
        
        // STT
        cout << left << setw(col1Width) << i + 1;
        
        // Tên VĐV (cắt bớt nếu quá dài)
        string tenVDV = danhSachKhongTrungLap[i].name;
        if (tenVDV.length() > col2Width - 1) {
            tenVDV = tenVDV.substr(0, col2Width - 3) + "..";
        }
        cout << setw(col2Width) << tenVDV;
        
        // Các thông tin còn lại
        cout << setw(col3Width) << danhSachKhongTrungLap[i].soTranThang
             << setw(col4Width) << danhSachKhongTrungLap[i].soTranThua
             << setw(col5Width) << danhSachKhongTrungLap[i].vongBiLoai;
    }
}

void XacDinhKetQuaCuoiCung() {
    if (soTranDaDau < soTranToiDa) return;

    system("cls");
    VeKhung(20, 2, 80, 10, 14);
    DoiMau(14);
    ToaDo(49, 3);
    cout << "| KET QUA CUOI CUNG |";
    DoiMau(15);

    vector<Player> top4;
    for (const auto& vdv : danhSachVDV) {
        if (vdv.xepHang >= 1 && vdv.xepHang <= 4) {
            top4.push_back(vdv);
        }
    }

    sort(top4.begin(), top4.end(), [](const Player& a, const Player& b) {
        return a.xepHang < b.xepHang;
    });

    VeCupVang(3, 5);
    VeCupVang(85, 5);

    DoiMau(14);
    ToaDo(37, 8);
    cout << "╔════════════ KET QUA CUOI CUNG ════════════╗";
    ToaDo(37, 9);
    cout << "║                                           ║";
    ToaDo(37, 10);
    cout << "║   VO DICH: " << left << setw(30) << top4[0].name;
    ToaDo (81, 10);
    cout << "║";
    ToaDo(37, 11);
    cout << "║   HANG NHI: " << left << setw(30) << top4[1].name;
    ToaDo (81, 11);
    cout << "║";
    ToaDo(37, 12);
    cout << "║   HANG BA: " << left << setw(30) << top4[2].name;
    ToaDo (81, 12);
    cout << "║";
    ToaDo(37, 13);
    cout << "║   HANG TU: " << left << setw(30) << top4[3].name;
    ToaDo (81, 13);
    cout << "║";
    ToaDo(37, 14);
    cout << "╚═══════════════════════════════════════════╝";

    ToaDo(40, 17);
    DoiMau(8);
    cout << "NHAN:";
    ToaDo(40, 18);
    cout << "[1] Quay lai menu chinh";
    getch();
}

void HienThiLichSuTranDau() {
    if (lichSuTranDau.empty()) {
        system("cls");
        VeKhung(35, 10, 50, 3, 12);
        DoiMau(12);
        ToaDo(40, 11);
        cout << "Chua co tran dau nao duoc ghi nhan!";
        sleep(2);
        return;
    }

    auto isBiLoai = [](int idVDV) {
        for (const auto& vdv : danhSachBiLoai) {
            if (vdv.danhSachGoc == idVDV) return true;
        }
        return false;
    };

    auto hienThiChiTietTran = [&](int tranSo) {
        system("cls");
        VeKhung(5, 2, 110, 28, 11);
        DoiMau(14);
        ToaDo(45, 0);
        cout << "╔════════ LICH SU TRAN " << tranSo << " ════════╗";
        
        if (tranSo > lichSuTranDau.size()) {
            DoiMau(12);
            ToaDo(47, 10);
            cout << "Tran " << tranSo << " chua co ket qua thi dau!";
        } 
        else {
            DoiMau(15);
            int yPos = 2;
            
            ToaDo(46, yPos++);
            cout << "Vong dau thu: " << tranSo << "| So cap dau: " << lichSuTranDau[tranSo-1].size();
            yPos++;

            for (const auto& tran : lichSuTranDau[tranSo-1]) {
                ToaDo(20, yPos++);
                cout << "Cap dau: " << tran.tenVDV1 << " vs " << tran.tenVDV2;
                
                ToaDo(20, yPos++);
                if (tran.ketQua) {
                    DoiMau(10);
                    cout << "Ket qua: " << tran.tenVDV1 << " thang";
                } else {
                    DoiMau(10);
                    cout << "Ket qua: " << tran.tenVDV2 << " thang";
                }
                DoiMau(15);
                yPos++;
            }
        }
        
        ToaDo(45, 33);
        DoiMau(8);
        cout << "NHAN:";
        ToaDo(45, 34);
        cout << "[1] Quay lai menu chinh";
        ToaDo(45, 35);
        cout << "[R] Xem tran khac";
    };

    while (true) {
        system("cls");
        VeKhung(5, 2, 110, 28, 11);
        DoiMau(14);
        ToaDo(45, 3);
        cout << "╔════════ CHON TRAN DE XEM ════════╗";
        
        DoiMau(15);
        ToaDo(47, 5);
        cout << "Danh sach cac tran (" << lichSuTranDau.size() << "/" << soTranToiDa << " da dau):";
        
        for (int i = 1; i <= soTranToiDa; i++) {
            ToaDo(55, 7 + i);
            if (i <= lichSuTranDau.size()) {
                cout << "Tran " << i << " (Da dau)";
            } else {
                cout << "Tran " << i << " (Chua dau)";
            }
        }
        
        ToaDo(53, 20);
        DoiMau(8);
        cout << "NHAN:";
        ToaDo(53, 21);
        cout << "[1-" << soTranToiDa << "] Xem tran cu the";
        ToaDo(53, 22);
        cout << "[R] Thoat chuc nang nay";
        
        char ch = getch();
        if (ch == 'r' || ch == 'R') {
            return;
        }
        else if (ch >= '1' && ch <= '9') {
            int tranChon = ch - '0';
            if (tranChon > 0 && tranChon <= soTranToiDa) {
                hienThiChiTietTran(tranChon);
                
                while (true) {
                    char ch2 = getch();
                    if (ch2 == '1') {
                        return;
                    }
                    else if (ch2 == 'r' || ch2 == 'R') {
                        break;
                    }
                }
            }
        }
    }
}

void VeMenuChinh(int LuaChon) {
    XoaVung(3, 2, 115, 34);
    VeHeaderFooter();
    VeKhung(35, 8, 50, 16, 14);

    string options[] = {
        "1 NHAP DANH SACH VDV & SO TRAN DAU",
        "2 SAP XEP CAP DAU CAC LUOT TRAN ",
        "3 NHAP KET QUA CAC LUOT TRAN    ",
        "4 KET QUA CAC TRAN DAU          ",
        "5 CAC VDV BI LOAI               ",
        "6 LICH SU TRAN DAU              ",
        "7 THOAT KHOI GIAI DAU           "
    };

    for (int i = 0; i < 7; ++i) {
        ToaDo(45, 15 + i * 2);
        if (LuaChon == i + 1) {
            DoiMau(11);
            cout << "> " << options[i];
        } else {
            DoiMau(7);
            cout << "  " << options[i];
        }
    }
}

int MenuChinh() {
    int LuaChon = 1;
    VeMenuChinh(LuaChon);
    
    while (true) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 72) {
                LuaChon--;
                if (LuaChon < 1) LuaChon = 7;
                VeMenuChinh(LuaChon);
            } else if (ch == 80) {
                LuaChon++;
                if (LuaChon > 7) LuaChon = 1;
                VeMenuChinh(LuaChon);
            } else if (ch == 13) {
                return LuaChon;
            }
        }
        Sleep(50);
    }
}