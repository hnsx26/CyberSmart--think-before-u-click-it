"""Cyber City: a small terminal game about safer online decisions."""

import random


DISTRICTS = [
    "Signal Gate", "Inbox Row", "Password Plaza", "Privacy Park",
    "Link Lane", "Market Square", "MFA Station", "Social Street",
    "Update Tower", "Cloud Crossing", "Firewall Yard", "Help Desk",
    "Cookie Corner", "Download Dock", "Data Depot", "Alert Avenue",
    "Backup Bridge", "Forensics Lab", "Trust Terminal", "Safe Harbor",
]

QUESTIONS = {
    "beginner": [
        ("PHISHING", "A delivery text asks you to sign in through a shortened link. What is safest?", [
            "Open the link to check the tracking number",
            "Visit the delivery company's official site or app yourself",
            "Reply with your email address",
        ], 1, "Unexpected links can lead to fake login pages. Visit a site or app you already trust."),
        ("PASSWORDS", "One site reports a breach. You reused that password elsewhere. What now?", [
            "Change it anywhere you reused it and make each password unique",
            "Wait for suspicious activity",
            "Change it only on the breached site",
        ], 0, "Attackers try leaked passwords on other services. Unique passwords limit the damage."),
        ("PRIVACY", "A public quiz asks for your birthday, school, and first pet's name. What should you consider?", [
            "Those details may help someone guess account recovery answers",
            "It is safe if the quiz has many comments",
            "Only your birthday is sensitive",
        ], 0, "Personal details can help build a profile or guess security answers. Share less publicly."),
    ],
    "field": [
        ("ACCOUNT SECURITY", "A sign-in prompt appears, but you are not logging in. What should you do?", [
            "Approve it to make the notification stop",
            "Deny it, change your password, and review account activity",
            "Ignore it; prompts expire harmlessly",
        ], 1, "An unexpected approval request may mean someone has your password. Deny it and secure the account."),
        ("SOCIAL ENGINEERING", "A caller claiming to be IT asks you to install a remote support tool now. How do you verify?", [
            "Ask them to confirm your employee ID",
            "End the call and contact IT through a known channel",
            "Install it but do not share your screen",
        ], 1, "Use an independent, known contact route. Caller ID and details can be faked."),
        ("NETWORKS", "You need to check your bank on open public Wi-Fi. Which option is safest?", [
            "Use mobile data or a trusted VPN",
            "Use Wi-Fi if the network name looks official",
            "Turn off Bluetooth and use the Wi-Fi",
        ], 0, "A familiar network name is not proof of legitimacy. Prefer mobile data or a trusted VPN."),
    ],
    "expert": [
        ("EVIDENCE", "A laptop may be compromised and needs preserving. What is the best first step?", [
            "Browse files to find suspicious programs",
            "Follow response instructions, document actions, and avoid altering evidence",
            "Run cleanup software immediately",
        ], 1, "Unplanned actions can change evidence. Follow the response plan and involve the authorized team."),
        ("LOG ANALYSIS", "Several failed logins are followed by a success from an unfamiliar location. What does that signal?", [
            "A potential compromise that merits investigation",
            "Proof that the user is traveling",
            "A harmless result if MFA is enabled",
        ], 0, "This is suspicious, not conclusive. Correlate with other logs and user confirmation."),
        ("RECOVERY", "Which backup practice best supports recovery from ransomware?", [
            "Keep backups writable from every endpoint",
            "Maintain tested, protected backups separated from ordinary access",
            "Back up only after an incident is detected",
        ], 1, "Protected, isolated backups are harder to alter. Test restores regularly."),
    ],
}


def read_choice(prompt, allowed):
    while True:
        try:
            choice = input(prompt).strip().lower()
        except EOFError:
            return None
        if choice in allowed:
            return choice
        print("Please enter one of: " + ", ".join(sorted(allowed)))


def play():
    print("\nCYBERSMART // CYBER CITY")
    print("Roll, explore, and make safer digital decisions.\n")
    try:
        name = input("Agent name: ").strip() or "Agent"
    except EOFError:
        return

    print("\nChoose difficulty: 1) Beginner  2) Field  3) Expert")
    selected = read_choice("> ", {"1", "2", "3"})
    if selected is None:
        return
    level = {"1": "beginner", "2": "field", "3": "expert"}[selected]
    deck = []
    position = 0
    score = 0
    safe_calls = 0
    turn = 0

    print(f"\nWelcome to Cyber City, {name}. Type h during a challenge for a hint.")
    while position < len(DISTRICTS) - 1:
        if input("\nPress Enter to roll (or q to quit): ").strip().lower() == "q":
            print("Mission paused. Keep thinking before you click.")
            return

        roll = random.randint(1, 6)
        position = min(len(DISTRICTS) - 1, position + roll)
        turn += 1
        print(f"Turn {turn}: rolled {roll}. You reached {DISTRICTS[position]}.")

        if not deck:
            deck = QUESTIONS[level].copy()
            random.shuffle(deck)
        category, prompt, answers, correct, explanation = deck.pop()
        print(f"\n[{category}] {prompt}")
        for index, answer in enumerate(answers, 1):
            print(f"  {index}. {answer}")

        hint_used = False
        while True:
            answer = read_choice("Answer 1-3 (or h for hint): ", {"1", "2", "3", "h"})
            if answer is None:
                return
            if answer == "h":
                if hint_used:
                    print("Maya: Take your time and check the source.")
                else:
                    hint_used = True
                    wrong = next(index for index in range(3) if index != correct)
                    print(f"Maya: One option to reconsider is: {answers[wrong]}")
                continue
            if int(answer) - 1 == correct:
                score += 100
                safe_calls += 1
                print("Good call! +100 points.")
            else:
                score = max(0, score - 20)
                print("Not quite. Review the signal.")
            print("Maya: " + explanation)
            break

    print(f"\nDISTRICT SECURED, {name.upper()}!")
    print(f"Signal score: {score:03d}  |  Safe calls: {safe_calls:02d}  |  Turns: {turn:02d}")


if __name__ == "__main__":
    play()