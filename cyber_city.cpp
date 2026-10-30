#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <string>
#include <vector>

struct Question {
    std::string category;
    std::string prompt;
    std::array<std::string, 3> answers;
    int correct;
    std::string explanation;
};

const std::array<std::string, 20> districts = {
    "Signal Gate", "Inbox Row", "Password Plaza", "Privacy Park", "Link Lane",
    "Market Square", "MFA Station", "Social Street", "Update Tower", "Cloud Crossing",
    "Firewall Yard", "Help Desk", "Cookie Corner", "Download Dock", "Data Depot",
    "Alert Avenue", "Backup Bridge", "Forensics Lab", "Trust Terminal", "Safe Harbor"
};

const std::vector<Question> beginner = {
    {"PHISHING", "A delivery text asks you to sign in through a shortened link. What is safest?",
     {"Open the link to check the tracking number", "Visit the delivery company's official site or app yourself", "Reply with your email address"},
     1, "Unexpected links can lead to fake login pages. Visit a site or app you already trust."},
    {"PASSWORDS", "One site reports a breach. You reused that password elsewhere. What now?",
     {"Change it anywhere you reused it and make each password unique", "Wait for suspicious activity", "Change it only on the breached site"},
     0, "Attackers try leaked passwords on other services. Unique passwords limit the damage."},
    {"PRIVACY", "A public quiz asks for your birthday, school, and first pet's name. What should you consider?",
     {"Those details may help someone guess account recovery answers", "It is safe if the quiz has many comments", "Only your birthday is sensitive"},
     0, "Personal details can help build a profile or guess security answers. Share less publicly."}
};

const std::vector<Question> field = {
    {"ACCOUNT SECURITY", "A sign-in prompt appears, but you are not logging in. What should you do?",
     {"Approve it to make the notification stop", "Deny it, change your password, and review account activity", "Ignore it; prompts expire harmlessly"},
     1, "An unexpected approval request may mean someone has your password. Deny it and secure the account."},
    {"SOCIAL ENGINEERING", "A caller claiming to be IT asks you to install a remote support tool now. How do you verify?",
     {"Ask them to confirm your employee ID", "End the call and contact IT through a known channel", "Install it but do not share your screen"},
     1, "Use an independent, known contact route. Caller ID and details can be faked."},
    {"NETWORKS", "You need to check your bank on open public Wi-Fi. Which option is safest?",
     {"Use mobile data or a trusted VPN", "Use Wi-Fi if the network name looks official", "Turn off Bluetooth and use the Wi-Fi"},
     0, "A familiar network name is not proof of legitimacy. Prefer mobile data or a trusted VPN."}
};

const std::vector<Question> expert = {
    {"EVIDENCE", "A laptop may be compromised and needs preserving. What is the best first step?",
     {"Browse files to find suspicious programs", "Follow response instructions, document actions, and avoid altering evidence", "Run cleanup software immediately"},
     1, "Unplanned actions can change evidence. Follow the response plan and involve the authorized team."},
    {"LOG ANALYSIS", "Several failed logins are followed by a success from an unfamiliar location. What does that signal?",
     {"A potential compromise that merits investigation", "Proof that the user is traveling", "A harmless result if MFA is enabled"},
     0, "This is suspicious, not conclusive. Correlate with other logs and user confirmation."},
    {"RECOVERY", "Which backup practice best supports recovery from ransomware?",
     {"Keep backups writable from every endpoint", "Maintain tested, protected backups separated from ordinary access", "Back up only after an incident is detected"},
     1, "Protected, isolated backups are harder to alter. Test restores regularly."}
};

bool readLine(const std::string& prompt, std::string& value) {
    std::cout << prompt;
    return static_cast<bool>(std::getline(std::cin, value));
}

int main() {
    std::cout << "\nCYBERSMART // CYBER CITY\n"
              << "Roll, explore, and make safer digital decisions.\n\n";

    std::string name;
    if (!readLine("Agent name: ", name)) return 0;
    if (name.empty()) name = "Agent";

    std::cout << "\nChoose difficulty: 1) Beginner  2) Field  3) Expert\n";
    std::string selection;
    if (!readLine("> ", selection)) return 0;
    while (selection != "1" && selection != "2" && selection != "3") {
        if (!readLine("Enter 1, 2, or 3: ", selection)) return 0;
    }

    const std::vector<Question>* source = &beginner;
    if (selection == "2") source = &field;
    if (selection == "3") source = &expert;
    std::vector<Question> deck;
    int position = 0;
    int score = 0;
    int safeCalls = 0;
    int turn = 0;
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> die(1, 6);

    std::cout << "\nWelcome to Cyber City, " << name << ". Type h during a challenge for a hint.\n";
    while (position < static_cast<int>(districts.size()) - 1) {
        std::string command;
        if (!readLine("\nPress Enter to roll (or q to quit): ", command)) return 0;
        if (command == "q" || command == "Q") {
            std::cout << "Mission paused. Keep thinking before you click.\n";
            return 0;
        }

        const int roll = die(generator);
        position = std::min(static_cast<int>(districts.size()) - 1, position + roll);
        ++turn;
        std::cout << "Turn " << turn << ": rolled " << roll << ". You reached " << districts[position] << ".\n";

        if (deck.empty()) {
            deck = *source;
            std::shuffle(deck.begin(), deck.end(), generator);
        }
        const Question question = deck.back();
        deck.pop_back();
        std::cout << "\n[" << question.category << "] " << question.prompt << '\n';
        for (std::size_t index = 0; index < question.answers.size(); ++index) {
            std::cout << "  " << index + 1 << ". " << question.answers[index] << '\n';
        }

        bool hintUsed = false;
        while (true) {
            std::string answer;
            if (!readLine("Answer 1-3 (or h for hint): ", answer)) return 0;
            if (answer == "h" || answer == "H") {
                if (hintUsed) {
                    std::cout << "Maya: Take your time and check the source.\n";
                } else {
                    hintUsed = true;
                    int wrong = 0;
                    while (wrong == question.correct) ++wrong;
                    std::cout << "Maya: One option to reconsider is: " << question.answers[wrong] << '\n';
                }
                continue;
            }
            if (answer != "1" && answer != "2" && answer != "3") {
                std::cout << "Enter 1, 2, 3, or h.\n";
                continue;
            }
            if (std::stoi(answer) - 1 == question.correct) {
                score += 100;
                ++safeCalls;
                std::cout << "Good call! +100 points.\n";
            } else {
                score = std::max(0, score - 20);
                std::cout << "Not quite. Review the signal.\n";
            }
            std::cout << "Maya: " << question.explanation << '\n';
            break;
        }
    }

    std::cout << "\nDISTRICT SECURED, " << name << "!\n"
              << "Signal score: " << score << "  |  Safe calls: " << safeCalls
              << "  |  Turns: " << turn << '\n';
    return 0;
}