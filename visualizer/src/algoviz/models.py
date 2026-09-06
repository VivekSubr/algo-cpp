from dataclasses import dataclass


@dataclass(frozen=True, slots=True)
class FunctionDefinition:
    name: str
    qualified_name: str
    signature: str
    line: int

    def as_dict(self) -> dict[str, object]:
        return {
            "name": self.name,
            "qualifiedName": self.qualified_name,
            "signature": self.signature,
            "line": self.line,
        }
